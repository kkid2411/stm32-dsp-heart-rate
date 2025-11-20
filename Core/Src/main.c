/* ========================== main.c (STM32F401RE) ===========================
 * Demo đo nhịp tim bằng cảm biến Pulse Sensor trên STM32F401RE Nucleo:
 *  - Đọc tín hiệu analog từ PA0 bằng ADC1 (chế độ liên tục).
 *  - Khử DC bằng trung bình trượt ~1 giây.
 *  - Lọc thông thấp IIR bậc 1 để làm mượt tín hiệu.
 *  - Phát hiện đỉnh (peak) bằng ngưỡng Schmitt động + state machine.
 *  - Từ khoảng RR (Beat-to-Beat) -> tính BPM trong miền thời gian.
 *  - Thu thập 1024 mẫu đã xử lý để tính FFT bằng CMSIS-DSP (arm_math.h).
 *  - Tìm đỉnh phổ trong dải nhịp tim và đổi sang BPM_FFT.
 *  - In dữ liệu dạng text để hiển thị trên Serial Monitor.
 *  - LED PA5 nháy mỗi khi phát hiện một nhịp tim hợp lệ.
 *
 *  Lưu ý: file nên lưu ở dạng UTF-8 (không BOM) để hiển thị tiếng Việt qua UART.
 * ==========================================================================*/

#include "main.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_uart.h"
#include "arm_math.h"   /* dùng CMSIS-DSP: arm_rfft_fast, arm_cmplx_mag, arm_max */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

/* -------------------- Handles phần cứng -------------------- */
ADC_HandleTypeDef  hadc1;   /* handle cho ADC1 */
UART_HandleTypeDef huart2;  /* handle cho USART2 (debug/plot) */

/* -------------------- Prototype các hàm cấu hình -------------------- */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
void Error_Handler(void);

/* ======================== Tham số thuật toán thời gian =====================
 * LOOP_DT_MS: chu kỳ vòng lặp chính (dùng HAL_Delay), đề bài yêu cầu ~500 ms.
 * PROCESS_FS_HZ: tần số xử lý thực tế bên trong (lấy mẫu ADC + xử lý).
 * SAMPLES_PER_BATCH: số mẫu xử lý mỗi vòng (PROCESS_FS_HZ * 0.5 s = 50).
 * ==========================================================================*/
#define LOOP_DT_MS               500u     /* vòng lặp chính ~2 Hz (mỗi 500 ms) */
#define PROCESS_FS_HZ            100u     /* "Fs logic" cho thuật toán & FFT    */
#define SAMPLES_PER_BATCH  ((PROCESS_FS_HZ * LOOP_DT_MS) / 1000u) /* = 50 mẫu */

/* ======================== Tham số thuật toán phát hiện nhịp ================
 * LED_ON_MS         : thời gian bật LED khi phát hiện nhịp (nháy LED).
 * REFRACTORY_MS     : thời gian "khóa" sau một đỉnh để tránh đếm nhịp đôi.
 * AVG_BPM_WINDOW    : số khoảng RR dùng để tính BPM trung bình.
 * WIN_MS            : độ dài cửa sổ trượt dùng để tìm min/max (biên độ p2p).
 * HEART_RATE_WARN_BPM: ngưỡng cảnh báo nhịp tim cao (chỉ để in thông báo).
 * TH_HI_PCT / TH_LO_PCT: ngưỡng Schmitt cao/thấp theo phần trăm biên độ p2p.
 * MIN_PEAK_TO_PEAK  : biên độ tối thiểu để chấp nhận có nhịp thật.
 * RR_MIN_MS / RR_MAX_MS: khoảng RR hợp lệ (giới hạn BPM đo được).
 * NO_PULSE_TIMEOUT_MS: quá thời gian này không có nhịp -> đặt BPM = 0.
 * ==========================================================================*/
#define LED_ON_MS                 30u      /* bật LED 30 ms khi phát hiện nhịp */
#define REFRACTORY_MS            330u     /* khóa hiển thị thêm nhịp trong 330 ms */
#define AVG_BPM_WINDOW             4u     /* trung bình 4 khoảng RR gần nhất */
#define WIN_MS                  1000u     /* cửa sổ 1 giây cho min/max biên độ */

#define HEART_RATE_WARN_BPM      110u     /* BPM > 110 -> in thêm cảnh báo */
#define TH_HI_PCT                 65u     /* ngưỡng cao = min + 65%*p2p */
#define TH_LO_PCT                 45u     /* ngưỡng thấp = min + 45%*p2p */
#define MIN_PEAK_TO_PEAK          40u     /* nếu biên độ < 40 -> bỏ qua nhiễu */
#define RR_MIN_MS                350u     /* RR nhỏ hơn -> quá nhanh, bỏ (≈171 BPM) */
#define RR_MAX_MS               2000u     /* RR lớn hơn -> quá chậm, bỏ (≈30 BPM) */
#define NO_PULSE_TIMEOUT_MS     2000u     /* 2 s không có nhịp -> xem như không đo */

/* ======================== PHẦN FFT bằng CMSIS-DSP ==========================
 * - FFT_N: số điểm FFT (1024 mẫu).
 * - fft_input : buffer lưu tín hiệu thời gian (sig) dạng float.
 * - fft_output: buffer phức sau RFFT (chiều dài N).
 * - fft_mag   : biên độ phổ cho từng bin (0..N/2-1).
 * - fft_index : vị trí hiện tại trong buffer.
 * - fft_ready : cờ báo đã đủ 1024 mẫu, sẵn sàng tính FFT.
 * - g_fft_peak_freq / g_fft_bpm: kết quả đỉnh phổ & BPM_FFT in ở main().
 * ==========================================================================*/
#define FFT_N           1024u
#define SAMPLE_RATE_HZ  PROCESS_FS_HZ   /* 100 Hz */

static float32_t fft_input[FFT_N];
static float32_t fft_output[FFT_N];
static float32_t fft_mag[FFT_N/2];
static uint32_t  fft_index = 0;
static bool      fft_ready = false;

/* Instance FFT của CMSIS-DSP */
static arm_rfft_fast_instance_f32 fft_instance;

/* GIÁ TRỊ FFT DÙNG ĐỂ IN ĐỊNH KỲ (2 lần/giây) */
static float32_t g_fft_peak_freq = 0.0f;  /* Hz */
static float32_t g_fft_bpm       = 0.0f;  /* BPM từ FFT */

static void FFT_Compute(void);   /* prototype hàm tính FFT và cập nhật kết quả */

/* --------- Redirect printf về UART2 (dùng cho debug + Serial Monitor) ----- */
int __io_putchar(int ch) {
  uint8_t c = (uint8_t)ch;
  HAL_UART_Transmit(&huart2, &c, 1, HAL_MAX_DELAY);
  return ch;
}
int fputc(int ch, FILE *f) { return __io_putchar(ch); }

/* ===================================================================== */
int main(void)
{
  HAL_Init();              /* Khởi tạo HAL và SysTick (HAL_GetTick) */
  SystemClock_Config();    /* Cấu hình clock hệ thống 84 MHz */
  MX_GPIO_Init();          /* Khởi tạo GPIO: PA5 (LED), PA0 (analog) */
  MX_ADC1_Init();          /* Khởi tạo ADC1 đọc kênh PA0 */
  MX_USART2_UART_Init();   /* Khởi tạo UART2 để in dữ liệu */

  /* Khởi tạo FFT CMSIS-DSP */
  if (arm_rfft_fast_init_f32(&fft_instance, FFT_N) != ARM_MATH_SUCCESS) {
    printf("FFT init error\r\n");
  }

  /* Tắt buffer chuẩn của printf để in ra ngay (không bị dồn) */
  setvbuf(stdout, NULL, _IONBF, 0);

  /* Bắt đầu chuyển đổi ADC ở chế độ liên tục */
  if (HAL_ADC_Start(&hadc1) != HAL_OK) Error_Handler();

  /* ================= FIFO cho cửa sổ min/max ~1 s ================= */
  enum { BUF_CAP = (WIN_MS * PROCESS_FS_HZ) / 1000u + 4u }; /* ≈104 */
  uint16_t buf[BUF_CAP]; memset(buf, 0, sizeof(buf));
  uint16_t head = 0, count = 0;
  uint16_t win_min = 0xFFFF, win_max = 0;

  /* Biến tạm đọc ADC */
  uint32_t raw = 0;

  /* ================= Khử DC bằng trung bình trượt 1 giây ========== */
  uint32_t ma_sum = 0;
  uint16_t ma_buf[BUF_CAP]; memset(ma_buf, 0, sizeof(ma_buf));
  uint16_t ma_head = 0, ma_cnt = 0;

  /* Low-pass IIR bậc 1: y[n] = 0.85*y[n-1] + 0.15*x[n] */
  int32_t  lp = 0;

  /* ================= Biến tính nhịp tim trong miền thời gian ========== */
  uint32_t lastBeatMs = 0, prevBeatMs = 0;
  uint32_t rr_buf[AVG_BPM_WINDOW] = {0};
  uint8_t  rr_cnt = 0, rr_idx = 0;
  uint32_t bpm_show = 0;

  /* ================= Quản lý LED ============================ */
  uint32_t ledOffAt = 0;

  /* ================= State machine phát hiện nhịp ===================== */
  typedef enum { WAIT_RISE = 0, WAIT_FALL = 1 } det_state_t;
  det_state_t st = WAIT_RISE;
  int32_t     cand_peak = INT32_MIN;
  uint32_t    cand_time = 0;

  /* ================= Vòng lặp vô hạn (main loop) ===================== */
  while (1)
  {
    /* ---------- Mỗi vòng chính xử lý SAMPLES_PER_BATCH mẫu ---------- */
    for (uint32_t k = 0; k < SAMPLES_PER_BATCH; ++k) {
      /* 1. Đọc 1 mẫu ADC (ADC đang chạy liên tục) */
      if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK) {
        raw = HAL_ADC_GetValue(&hadc1);           /* giá trị 0..4095 (12-bit) */
      } else {
        /* Trường hợp hiếm: timeout -> khởi động lại ADC rồi bỏ mẫu này */
        HAL_ADC_Stop(&hadc1);
        HAL_ADC_Start(&hadc1);
        continue;
      }

      /* 2. Khử DC bằng trung bình trượt 1 giây trên "raw" */
      if (ma_cnt < BUF_CAP) ma_cnt++;             /* chưa đầy thì tăng dần */
      ma_sum -= ma_buf[ma_head];                  /* trừ mẫu cũ ra khỏi tổng */
      ma_buf[ma_head] = (uint16_t)raw;            /* ghi mẫu mới */
      ma_sum += ma_buf[ma_head];                  /* cộng mẫu mới vào tổng */
      ma_head = (ma_head + 1) % BUF_CAP;          /* dịch vị trí ghi (vòng tròn) */
      uint16_t ma = (uint16_t)(ma_sum / (ma_cnt ? ma_cnt : 1)); /* giá trị nền DC */

      /* x_dc: tín hiệu đã khử DC, lấy raw - nền */
      int32_t x_dc = (int32_t)raw - (int32_t)ma;

      /* 3. Lọc thông thấp IIR bậc 1 để làm mượt (giảm nhiễu cao tần) */
      lp = ( (85 * lp) + (15 * x_dc) ) / 100;
      int32_t sig = lp;                           /* tín hiệu đã xử lý dùng cho detect & FFT */

      /* 4. Lưu tín hiệu đã xử lý vào buffer FFT (chuẩn hoá về [-1,1] tương đối) */
      fft_input[fft_index++] = (float32_t)sig / 2048.0f;
      if (fft_index >= FFT_N) {
        fft_index = 0;        /* quay lại đầu buffer */
        fft_ready = true;     /* báo đã đủ 1024 mẫu để tính FFT */
      }

      /* 5. Cập nhật cửa sổ min/max cho tín hiệu (để tính biên độ p2p) */
      uint16_t sig_u = (uint16_t)(sig + 2048);

      if (count < BUF_CAP) count++;
      uint16_t old = buf[head];    /* mẫu sắp bị ghi đè */
      buf[head] = sig_u;
      head = (head + 1) % BUF_CAP;

      if (count == BUF_CAP) {
        /* buffer đã đầy: cần xử lý kỹ khi mẫu cũ chính là min hoặc max */
        if (old == win_min || old == win_max) {
          /* phải quét lại toàn bộ để tìm min/max mới */
          win_min = 0xFFFF;
          win_max = 0;
          for (uint16_t i = 0, idx = head; i < count; i++, idx=(idx+1)%BUF_CAP) {
            uint16_t v = buf[idx];
            if (v < win_min) win_min = v;
            if (v > win_max) win_max = v;
          }
        } else {
          /* chỉ cập nhật min/max theo mẫu mới */
          if (sig_u < win_min) win_min = sig_u;
          if (sig_u > win_max) win_max = sig_u;
        }
      } else {
        /* buffer chưa đầy: min/max chỉ dựa trên các mẫu đã có */
        if (sig_u < win_min) win_min = sig_u;
        if (sig_u > win_max) win_max = sig_u;
      }

      uint16_t pk2pk = (uint16_t)(win_max - win_min);  /* biên độ peak-to-peak */

      /* 6. Tính ngưỡng Schmitt (cao / thấp) dựa trên biên độ hiện tại */
      uint16_t th_hi = (uint16_t)(win_min + (uint32_t)pk2pk * TH_HI_PCT / 100);
      uint16_t th_lo = (uint16_t)(win_min + (uint32_t)pk2pk * TH_LO_PCT / 100);

      /* 7. Watchdog nhịp tim: nếu quá lâu không thấy nhịp -> BPM = 0 */
      uint32_t now = HAL_GetTick();   /* thời gian hệ thống (ms) */
      if (now - lastBeatMs > NO_PULSE_TIMEOUT_MS) {
        bpm_show   = 0;
        prevBeatMs = 0;
      }

      /* 8. State machine phát hiện nhịp tim (giữ nguyên như code cũ) */
      bool     accepted = false;      /* cờ báo vừa phát hiện một nhịp hợp lệ */
      uint16_t sig_w    = sig_u;      /* dùng phiên bản dương (offset) cho so sánh ngưỡng */

      switch (st) {
        case WAIT_RISE:
          /* Chờ tín hiệu vượt qua ngưỡng cao (th_hi) và biên độ đủ lớn */
          cand_peak = INT32_MIN;
          if (pk2pk >= MIN_PEAK_TO_PEAK && sig_w >= th_hi) {
            st = WAIT_FALL;
            cand_peak = sig;
            cand_time = now;
          }
          break;

        case WAIT_FALL:
          /* Đang theo dõi pha đỉnh: nếu còn tăng thì cập nhật đỉnh */
          if (sig > cand_peak) {
            cand_peak = sig;
            cand_time = now;
          }
          /* Khi tín hiệu rơi xuống dưới th_lo -> chốt đỉnh */
          if (sig_w <= th_lo) {
            /* Kiểm tra thời gian trơ (refractory) để tránh đếm đôi */
            if ((now - lastBeatMs) > REFRACTORY_MS) {
              accepted = true;       /* xác nhận đây là một nhịp mới */
              lastBeatMs = now;
            }
            st = WAIT_RISE;          /* quay lại chờ đỉnh tiếp theo */
          }
          /* Nếu bị kẹt quá lâu ở WAIT_FALL -> reset state machine */
          if (now - cand_time > 1200u) st = WAIT_RISE;
          break;
      }

      /* 9. Khi chốt một nhịp hợp lệ: cập nhật BPM & điều khiển LED */
      if (accepted) {
        /* Tính khoảng RR và BPM trung bình */
        if (prevBeatMs > 0) {
          uint32_t rr = now - prevBeatMs;      /* khoảng giữa 2 nhịp liên tiếp */
          if (rr >= RR_MIN_MS && rr <= RR_MAX_MS) {
            rr_buf[rr_idx] = rr;
            rr_idx = (rr_idx + 1) % AVG_BPM_WINDOW;
            if (rr_cnt < AVG_BPM_WINDOW) rr_cnt++;
            uint32_t sum = 0;
            for (uint8_t i = 0; i < rr_cnt; ++i) sum += rr_buf[i];
            uint32_t rr_avg = sum / rr_cnt;
            bpm_show = (rr_avg > 0) ? (60000u / rr_avg) : 0; /* BPM = 60 000 / RR(ms) */
          }
        }
        prevBeatMs = now;

        /* Mỗi lần có nhịp -> bật LED trong LED_ON_MS để nháy */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        ledOffAt = now + LED_ON_MS;

        /* In thông tin phát hiện nhịp (tuỳ chọn) */
        printf(">>> Phat hien nhip!  BPM:%lu%s\r\n",
               bpm_show,
               (bpm_show > HEART_RATE_WARN_BPM) ? "  (CANH BAO >110)" : "");
      }

      /* 10. Tắt LED đúng thời điểm, không dùng delay blocking */
      if (ledOffAt && now >= ledOffAt) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        ledOffAt = 0;
      }
    } /* ====== Kết thúc xử lý 1 batch 50 mẫu ====== */

    /* 11. Nếu đã thu đủ 1024 mẫu cho FFT thì gọi hàm FFT (CMSIS-DSP) */
    if (fft_ready) {
      fft_ready = false;
      FFT_Compute();   /* cập nhật g_fft_peak_freq, g_fft_bpm */
    }

    /* 12. In kết quả TỔNG HỢP 2 lần/giây:
     *  - Nhịp tim theo miền thời gian (bpm_show)
     *  - FFT_Peak (Hz) và FFT_BPM (FFT_Peak * 60)
     */
    printf("-> BPM:%lu FFT_Peak=%.1f Hz, FFT_BPM=%.1f\r\n",
           bpm_show,
           g_fft_peak_freq,
           g_fft_bpm);

    /* 13. Vòng lặp chính nghỉ 500 ms theo đúng yêu cầu đề bài */
    HAL_Delay(LOOP_DT_MS);
  }
}

/* ================== Hàm tính FFT bằng CMSIS-DSP (PHẦN FFT) ================*/
static void FFT_Compute(void)
{
  /* 1. RFFT nhanh (real FFT): fft_input -> fft_output */
  arm_rfft_fast_f32(&fft_instance, fft_input, fft_output, 0);

  /* 2. Tính biên độ |X(k)| cho k = 0..FFT_N/2-1 */
  arm_cmplx_mag_f32(fft_output, fft_mag, FFT_N/2);

  /* 3. Chỉ tìm đỉnh trong dải tần có thể là nhịp tim: 0.7–3 Hz */
  float32_t f_min = 0.7f;   /* Hz  ~ 42 BPM */
  float32_t f_max = 3.0f;   /* Hz  ~ 180 BPM */

  uint32_t k_min = (uint32_t)(f_min * (float32_t)FFT_N / (float32_t)SAMPLE_RATE_HZ);
  uint32_t k_max = (uint32_t)(f_max * (float32_t)FFT_N / (float32_t)SAMPLE_RATE_HZ);

  if (k_min < 1) k_min = 1;                        /* bỏ DC */
  if (k_max >= FFT_N/2) k_max = FFT_N/2 - 1;

  float32_t maxVal;
  uint32_t  maxIdxRel;

  /* arm_max_f32 trên đoạn [k_min..k_max] */
  arm_max_f32(&fft_mag[k_min], (k_max - k_min + 1), &maxVal, &maxIdxRel);
  uint32_t peak_k = k_min + maxIdxRel;

  /* 4. từ chỉ số peak_k -> tần số đỉnh (Hz) -> BPM_FFT */
  float32_t df       = (float32_t)SAMPLE_RATE_HZ / (float32_t)FFT_N;   /* độ phân giải tần số */
  float32_t peakFreq = df * (float32_t)peak_k;                         /* Hz */
  float32_t bpm_fft  = peakFreq * 60.0f;                               /* BPM */

  /* 5. cập nhật biến global để main() in định kỳ */
  g_fft_peak_freq = peakFreq;
  g_fft_bpm       = bpm_fft;
}

/* ================= GPIO: PA5 (LED), PA0 (analog cho ADC1_IN0) =============*/
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* PA5: LED LD2 (trên board Nucleo) */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PA0: Analog input cho ADC1_CH0 (kết nối Pulse Sensor) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* ================= Cấu hình ADC1 đọc PA0 ================================ */
static void MX_ADC1_Init(void)
{
  __HAL_RCC_ADC1_CLK_ENABLE();

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4; /* 84MHz / 4 = 21MHz */
  hadc1.Init.Resolution            = ADC_RESOLUTION_12B;           /* độ phân giải 12-bit */
  hadc1.Init.ScanConvMode          = DISABLE;                      /* không quét nhiều kênh */
  hadc1.Init.ContinuousConvMode    = ENABLE;                       /* chạy liên tục */
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion       = 1;
  hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK) Error_Handler();

  /* Cấu hình kênh ADC1: Channel 0 (PA0) */
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel      = ADC_CHANNEL_0;
  sConfig.Rank         = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; /* thời gian sample dài hơn nếu trở nguồn cao */
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) Error_Handler();
}

/* ================= Cấu hình USART2: baud 115200 trên PA2/PA3 ==============*/
static void MX_USART2_UART_Init(void)
{
  __HAL_RCC_USART2_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Cấu hình GPIO cho USART2 (PA2: TX, PA3: RX) */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Cấu hình UART2 */
  huart2.Instance          = USART2;
  huart2.Init.BaudRate     = 115200;
  huart2.Init.WordLength   = UART_WORDLENGTH_8B;
  huart2.Init.StopBits     = UART_STOPBITS_1;
  huart2.Init.Parity       = UART_PARITY_NONE;
  huart2.Init.Mode         = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK) Error_Handler();
}

/* ================= Cấu hình System Clock 84 MHz (HSI + PLL) ===============*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;   /* SYSCLK = 84 MHz */
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) Error_Handler();
}

/* ================= Error Handler: chớp LED khi có lỗi nghiêm trọng ========*/
void Error_Handler(void)
{
  __disable_irq();
  while (1) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(1000);
  }
}
/* ======================== end of file ===================================== */

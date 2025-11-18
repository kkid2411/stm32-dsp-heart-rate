/* ========================== main.c (STM32F401RE) ===========================
 * Demo đo nhịp tim bằng cảm biến Pulse Sensor trên STM32F401RE Nucleo:
 *  - Đọc tín hiệu analog từ PA0 bằng ADC1 (chế độ liên tục).
 *  - Khử DC bằng trung bình trượt ~1 giây.
 *  - Lọc thông thấp IIR bậc 1 để làm mượt tín hiệu.
 *  - Phát hiện đỉnh (peak) bằng ngưỡng Schmitt động + state machine.
 *  - Từ khoảng RR (Beat-to-Beat) -> tính BPM trong miền thời gian.
 *  - Thu thập 1024 mẫu đã xử lý để tính DFT (FFT thủ công) -> tìm đỉnh phổ.
 *  - In dữ liệu dạng "key:value" để hiển thị trên Serial Plotter (Arduino IDE).
 *  - LED PA5 nháy mỗi khi phát hiện một nhịp tim hợp lệ.
 *
 *  Lưu ý: file nên lưu ở dạng UTF-8 (không BOM) để hiển thị tiếng Việt qua UART.
 * ==========================================================================*/

#include "main.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_uart.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>   /* dùng cosf, sinf, sqrtf cho DFT thủ công */

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
#define PROCESS_FS_HZ            100u     /* "Fs logic" cho thuật toán: 100 Hz  */
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

/* ======================== PHẦN 4: DFT/FFT =========================
 * Không dùng arm_math.h, ta tự cài đặt DFT 1 phía (0..Fs/2).
 * - FFT_N: số điểm DFT (1024 mẫu).
 * - fft_in : buffer lưu tín hiệu thời gian đã xử lý (sig).
 * - fft_re, fft_im, fft_mag: phần thực, ảo, và biên độ phổ cho từng bin.
 * - fft_index: vị trí hiện tại trong buffer.
 * - fft_ready: cờ báo đã đủ 1024 mẫu, sẵn sàng tính DFT.
 * ==================================================================*/
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define FFT_N 1024u   /* số điểm DFT – như trong tài liệu hướng dẫn */

static float fft_in[FFT_N];      /* mẫu tín hiệu thời gian sau xử lý */
static float fft_re[FFT_N/2];    /* phần thực phổ cho k = 0..FFT_N/2-1 */
static float fft_im[FFT_N/2];    /* phần ảo phổ */
static float fft_mag[FFT_N/2];   /* biên độ phổ |X(k)| */
static uint32_t fft_index = 0;   /* con trỏ ghi mẫu vào fft_in */
static bool     fft_ready = false; /* true khi đủ 1024 mẫu để tính DFT */

static void FFT_Compute(void);   /* prototype hàm tính DFT và in kết quả */

/* --------- Redirect printf về UART2 (dùng cho debug + Serial Plotter) ----- */
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

  /* Tắt buffer chuẩn của printf để in ra ngay (không bị dồn) */
  setvbuf(stdout, NULL, _IONBF, 0);

  /* Bắt đầu chuyển đổi ADC ở chế độ liên tục */
  if (HAL_ADC_Start(&hadc1) != HAL_OK) Error_Handler();

  /* ================= FIFO cho cửa sổ min/max ~1 s =================
   * BUF_CAP = số mẫu trong 1 s ở Fs=PROCESS_FS_HZ (~100) + một chút dư.
   * buf      : lưu tín hiệu đã offset (sig_u) để tính min/max trượt.
   * head     : vị trí ghi tiếp theo trong buffer (vòng tròn).
   * count    : số mẫu hiện có trong buffer (chưa đầy thì < BUF_CAP).
   * win_min/max: min/max đang theo dõi trong cửa sổ 1 s.
   * =================================================================*/
  enum { BUF_CAP = (WIN_MS * PROCESS_FS_HZ) / 1000u + 4u }; /* ≈104 */
  uint16_t buf[BUF_CAP]; memset(buf, 0, sizeof(buf));
  uint16_t head = 0, count = 0;
  uint16_t win_min = 0xFFFF, win_max = 0;

  /* Biến tạm đọc ADC */
  uint32_t raw = 0;

  /* ================= Khử DC bằng trung bình trượt 1 giây ==========
   * ma_sum : tổng các phần tử trong buffer trung bình.
   * ma_buf : buffer trung bình trượt.
   * ma_head: vị trí ghi mới trong ma_buf.
   * ma_cnt : số mẫu hiện đang có (tăng dần tới BUF_CAP).
   * ma     : giá trị trung bình hiện tại (nền DC).
   * =================================================================*/
  uint32_t ma_sum = 0;
  uint16_t ma_buf[BUF_CAP]; memset(ma_buf, 0, sizeof(ma_buf));
  uint16_t ma_head = 0, ma_cnt = 0;

  /* Low-pass IIR bậc 1 (sau khử DC): y[n] = 0.85*y[n-1] + 0.15*x[n]
   * lp: tín hiệu sau lọc, làm mượt nhiễu cao tần.
   * Dùng kiểu int32_t vì có thể âm/dương (đã khử DC).
   */
  int32_t  lp = 0;

  /* ================= Biến tính nhịp tim trong miền thời gian ==========
   * lastBeatMs : thời điểm (ms) phát hiện nhịp gần nhất.
   * prevBeatMs : mốc để đo RR = now - prevBeatMs.
   * rr_buf     : buffer lưu một số khoảng RR để lấy trung bình.
   * rr_cnt     : số phần tử hợp lệ trong rr_buf.
   * rr_idx     : chỉ số vòng tròn để ghi RR mới.
   * bpm_show   : BPM trung bình hiện tại (hiển thị ra UART/plot).
   * ====================================================================*/
  uint32_t lastBeatMs = 0, prevBeatMs = 0;
  uint32_t rr_buf[AVG_BPM_WINDOW] = {0};
  uint8_t  rr_cnt = 0, rr_idx = 0;
  uint32_t bpm_show = 0;

  /* ================= Quản lý LED & logging ============================
   * ledOffAt  : thời điểm tắt LED (millis), 0 nghĩa là LED đang tắt.
   * lastPrint : dùng để in debug ĐỊNH KỲ (~10 Hz) chứ không phải mỗi mẫu.
   * ====================================================================*/
  uint32_t ledOffAt = 0;
  uint32_t lastPrint = 0;

  /* ================= State machine phát hiện nhịp =====================
   * Ta dùng 2 trạng thái:
   *  - WAIT_RISE: chờ tín hiệu vượt qua ngưỡng cao (th_hi) để bắt đầu đỉnh.
   *  - WAIT_FALL: sau khi qua th_hi, chờ tín hiệu rơi xuống lại qua th_lo
   *               để "chốt" đỉnh, tránh nhiễu lặt vặt.
   * cand_peak  : giá trị đỉnh tạm thời (sig) trong pha WAIT_FALL.
   * cand_time  : thời điểm đo được đỉnh lớn nhất trong pha WAIT_FALL.
   * ====================================================================*/
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
      int32_t sig = lp;                           /* tín hiệu đã xử lý dùng cho detect */

      /* 4. Lưu tín hiệu đã xử lý vào buffer FFT (để phân tích phổ sau này) */
      fft_in[fft_index++] = (float)sig;
      if (fft_index >= FFT_N) {
        fft_index = 0;        /* quay lại đầu buffer */
        fft_ready = true;     /* báo đã đủ 1024 mẫu để tính DFT */
      }

      /* 5. Cập nhật cửa sổ min/max cho tín hiệu (để tính biên độ p2p) */
      /*    Để dùng kiểu uint16_t, ta dịch sig lên bằng 2048 (offset) */
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

      /* 8. State machine phát hiện nhịp tim */
      bool accepted = false;      /* cờ báo vừa phát hiện một nhịp hợp lệ */
      uint16_t sig_w = sig_u;     /* dùng phiên bản dương (offset) cho so sánh ngưỡng */

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

        /* In thông tin phát hiện nhịp (dùng xem trên Serial Monitor) */
        printf(">>> Phat hien nhip!  BPM:%lu%s\r\n",
               bpm_show,
               (bpm_show > HEART_RATE_WARN_BPM) ? "  (CANH BAO >110)" : "");
      }

      /* 10. In dữ liệu định kỳ ~10 Hz, dạng phù hợp với Serial Plotter
       *  Format: "tho:... nen:... tin:... Min:... Max:... BPMx20:..."
       *  - tho   : giá trị ADC thô (0..4095).
       *  - nen   : giá trị nền (trung bình trượt).
       *  - tin   : tín hiệu sau khử DC + lọc (sig).
       *  - Min/Max: giá trị min/max trong cửa sổ 1 s.
       *  - BPMx20: BPM nhân 20 để sóng nhịp tim nổi rõ hơn trong Plotter.
       */
      if (now - lastPrint >= 100u) {
        lastPrint = now;
        printf("tho:%lu nen:%u tin:%ld Min:%u Max:%u BPMx20:%lu\r\n",
               raw, ma, (long)sig, win_min, win_max, bpm_show * 20u);
      }

      /* 11. Tắt LED đúng thời điểm, không dùng delay blocking */
      if (ledOffAt && now >= ledOffAt) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        ledOffAt = 0;
      }
    } /* ====== Kết thúc xử lý 1 batch 50 mẫu ====== */

    /* 12. Nếu đã thu đủ 1024 mẫu cho FFT thì gọi hàm DFT */
    if (fft_ready) {
      fft_ready = false;
      FFT_Compute();   /* In ra: FFT_Peak=... Hz, FFT_BPM=... (Serial Monitor) */
    }

    /* 13. Vòng lặp chính nghỉ 500 ms theo đúng yêu cầu đề bài */
    HAL_Delay(LOOP_DT_MS);
  }
}

/* ================== Hàm tính DFT/FFT (PHẦN 4) =============================
 * Hàm FFT_Compute():
 *  - Sử dụng công thức DFT trực tiếp (không phải FFT tối ưu).
 *  - Tính phần thực/ảo cho k = 0..FFT_N/2 - 1 (nửa phổ, do tín hiệu là thực).
 *  - Tính biên độ phổ |X(k)| = sqrt(Re^2 + Im^2).
 *  - Tìm bin có biên độ lớn nhất (bỏ k=0 để tránh DC).
 *  - Chuyển bin -> tần số -> BPM_FFT, rồi in ra UART.
 * ==========================================================================*/
static void FFT_Compute(void)
{
  /* B1: tính DFT cho từng bin k (0..FFT_N/2-1) */
  for (uint32_t k = 0; k < FFT_N/2; ++k) {
    float sumRe = 0.0f;
    float sumIm = 0.0f;
    for (uint32_t n = 0; n < FFT_N; ++n) {
      float angle = 2.0f * M_PI * (float)k * (float)n / (float)FFT_N;
      float c = cosf(angle);
      float s = sinf(angle);
      float x = fft_in[n];
      sumRe += x * c;
      sumIm -= x * s;  /* e^{-jωn} = cos - j sin */
    }
    fft_re[k]  = sumRe;
    fft_im[k]  = sumIm;
    fft_mag[k] = sqrtf(sumRe * sumRe + sumIm * sumIm);
  }

  /* B2: tìm đỉnh phổ (bỏ k=0 vì đó là thành phần DC) */
  uint32_t peak_k = 1;
  float    peak_val = fft_mag[1];
  for (uint32_t k = 2; k < FFT_N/2; ++k) {
    if (fft_mag[k] > peak_val) {
      peak_val = fft_mag[k];
      peak_k   = k;
    }
  }

  /* B3: từ chỉ số peak_k -> tần số đỉnh (Hz) -> BPM_FFT */
  float df       = (float)PROCESS_FS_HZ / (float)FFT_N;   /* độ phân giải tần số */
  float peakFreq = df * (float)peak_k;                    /* Hz */
  float bpm_fft  = peakFreq * 60.0f;                      /* BPM */

  /* B4: in ra kết quả FFT dưới dạng text để quan sát trên Serial Monitor */
  printf("FFT_Peak=%.1f Hz, FFT_BPM=%.1f\r\n", peakFreq, bpm_fft);
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

# STM32F401RE – Pulse Sensor Heart Rate Monitor (Time-domain + FFT với CMSIS-DSP)

Dự án này hiện thực đo nhịp tim bằng **Pulse Sensor** trên kit **Nucleo-F401RE**, gồm:

- Đo tín hiệu analog từ cảm biến trên chân **PA0 (ADC1_IN0)**.  
- Khử DC bằng trung bình trượt ~1 giây.  
- Lọc thông thấp IIR bậc 1 để làm mượt tín hiệu.  
- Phát hiện nhịp tim trong **miền thời gian** bằng ngưỡng Schmitt + state machine.  
- Tính **BPM** từ khoảng cách giữa các đỉnh RR (Beat-to-Beat).  
- Thu 1024 mẫu đã xử lý và tính **FFT** bằng thư viện **CMSIS-DSP (`arm_math.h`)** để tìm đỉnh phổ và suy ra **BPM_FFT**.  
- Gửi kết quả qua UART (USART2) để xem trên **Serial Monitor** và nháy **LED PA5** mỗi khi phát hiện 1 nhịp tim hợp lệ.

---

## 1. Phần cứng

- **MCU**: STM32F401RE (Nucleo-F401RE board).  
- **Cảm biến nhịp tim**: Pulse Sensor (hoặc cảm biến tương tự xuất tín hiệu analog).  
- **Kết nối chính**:
  - Pulse Sensor **OUT → PA0** (ADC1_IN0).  
  - GND cảm biến → GND board.  
  - VCC cảm biến (3.3V hoặc 5V tùy loại, khuyến nghị 3.3V nếu tín hiệu đưa trực tiếp vào ADC).  
  - **LED on-board**: PA5 (LD2) – đã có sẵn trên kit Nucleo.  
  - **UART2**:
    - PA2: TX (Nucleo → PC / USB-UART on-board).  
    - PA3: RX (không dùng trong project này, nhưng vẫn cấu hình sẵn).  

Trên Nucleo-F401RE, UART2 đã nối sẵn với cổng USB ST-LINK, chỉ cần cắm cáp USB và mở Serial Monitor trên PC.

---

## 2. Phần mềm & Thư viện

- **IDE đề nghị**:
  - STM32CubeIDE (hoặc bất kỳ toolchain ARM GCC tương thích).
- **Thư viện**:
  - HAL của STM32F4 (đã dùng trong `main.c`).
  - **CMSIS-DSP (`arm_math.h`)**:
    - Cần đảm bảo đã:
      - Thêm đường dẫn include tới **CMSIS/Core** và **CMSIS/DSP**.
      - Link đúng file thư viện hoặc file nguồn DSP (ví dụ: `arm_rfft_fast_f32.c`, `arm_cmplx_mag_f32.c`, … hoặc static lib tương ứng cho Cortex-M4F).

---

## 3. Tham số quan trọng trong code

```c
#define PROCESS_FS_HZ      100u     // Tần số xử lý & lấy mẫu logic: ~100 Hz
#define LOOP_DT_MS         500u     // Vòng lặp chính: 500 ms (2 Hz)
#define FFT_N              1024u    // Số điểm FFT
Mỗi 500 ms, vòng while(1) xử lý 50 mẫu (100 Hz × 0.5 s).

Sau khi thu đủ 1024 mẫu đã xử lý → gọi FFT_Compute().

Các ngưỡng/giới hạn dùng cho thuật toán phát hiện nhịp tim:

c
Sao chép mã
#define LED_ON_MS                 30u
#define REFRACTORY_MS            330u
#define AVG_BPM_WINDOW             4u
#define WIN_MS                  1000u

#define HEART_RATE_WARN_BPM      110u
#define TH_HI_PCT                 65u
#define TH_LO_PCT                 45u
#define MIN_PEAK_TO_PEAK          40u
#define RR_MIN_MS                350u   // ~171 BPM
#define RR_MAX_MS               2000u   // ~30 BPM
#define NO_PULSE_TIMEOUT_MS     2000u
Bạn có thể tinh chỉnh các giá trị này nếu muốn thuật toán nhạy hơn / bớt nhạy, hoặc ưu tiên bỏ nhiễu mạnh hơn.

4. Luồng xử lý tín hiệu
4.1. Lấy mẫu & Tiền xử lý
Trong vòng lặp:

Đọc ADC từ PA0 (ADC1 hoạt động chế độ liên tục):



HAL_ADC_PollForConversion(&hadc1, 1);
raw = HAL_ADC_GetValue(&hadc1);  // 0..4095 (12-bit)
Khử DC bằng trung bình trượt ~1 giây:

Lưu các mẫu raw vào buffer ma_buf.

Tính tổng ma_sum, chia cho số mẫu hiện có -> ma (nền DC).

Tín hiệu đã khử DC:



x_dc = raw - ma;
Lọc thông thấp IIR bậc 1 để làm mượt:


// y[n] = 0.85 * y[n-1] + 0.15 * x[n]
lp = (85 * lp + 15 * x_dc) / 100;
sig = lp;
Đưa dữ liệu vào buffer FFT:

Chuẩn hóa tương đối về khoảng [-1, 1] rồi lưu:


fft_input[fft_index++] = (float32_t)sig / 2048.0f;
Khi fft_index đạt 1024 → fft_ready = true.

Cửa sổ trượt min/max 1 s:

Dùng buffer vòng buf[] để lưu sig_u = sig + 2048.

Cập nhật win_min, win_max → biên độ peak-to-peak:


pk2pk = win_max - win_min;
Tính ngưỡng Schmitt động theo p2p:


th_hi = win_min + pk2pk * TH_HI_PCT / 100;
th_lo = win_min + pk2pk * TH_LO_PCT / 100;
4.2. Thuật toán phát hiện nhịp trong miền thời gian
Sử dụng state machine 2 trạng thái:

WAIT_RISE – chờ tín hiệu vượt ngưỡng cao th_hi với biên độ đủ lớn (pk2pk >= MIN_PEAK_TO_PEAK).

WAIT_FALL – sau khi qua th_hi, theo dõi đỉnh, đến khi tín hiệu rơi xuống dưới th_lo thì chốt đỉnh.

Mỗi khi phát hiện 1 đỉnh hợp lệ:

Tính khoảng RR = now - prevBeatMs.

Chỉ chấp nhận nếu RR nằm trong [RR_MIN_MS, RR_MAX_MS].

Lưu RR vào rr_buf[], tính RR trung bình và suy ra:


bpm_show = 60000 / rr_avg;
Bật LED PA5 trong LED_ON_MS ms:


HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
ledOffAt = now + LED_ON_MS;
In ra UART thông tin phát hiện nhịp:


>>> Phat hien nhip!  BPM:xx (CANH BAO >110)
4.3. FFT với CMSIS-DSP (arm_math.h)
Khi fft_ready == true, hàm FFT_Compute() được gọi:

FFT thực một chiều:


arm_rfft_fast_f32(&fft_instance, fft_input, fft_output, 0);
Tính biên độ phổ:


arm_cmplx_mag_f32(fft_output, fft_mag, FFT_N/2);
Chỉ tìm đỉnh trong dải tần nhịp tim có ý nghĩa:

Dải: 0.7 – 3 Hz (~42 – 180 BPM).

Đổi dải tần sang dải chỉ số bin [k_min, k_max].

Dùng arm_max_f32() để tìm bin có biên độ lớn nhất:


arm_max_f32(&fft_mag[k_min], (k_max - k_min + 1), &maxVal, &maxIdxRel);
peak_k = k_min + maxIdxRel;
Đổi bin ra tần số:


df       = SAMPLE_RATE_HZ / FFT_N;   // Độ phân giải tần số
peakFreq = df * peak_k;              // Hz
bpm_fft  = peakFreq * 60.0f;         // BPM
Lưu vào biến global:


g_fft_peak_freq = peakFreq;
g_fft_bpm       = bpm_fft;
Trong main(), cứ mỗi 500 ms, chương trình in dòng tổng hợp:


-> BPM:xx FFT_Peak=yy.y Hz, FFT_BPM=zz.z
5. UART & Serial Monitor
UART dùng USART2, cấu hình:

Baud rate: 115200

8 data bits, không parity, 1 stop bit (8N1).

Dữ liệu in ra dạng text, ví dụ:


>>> Phat hien nhip!  BPM:75
-> BPM:75 FFT_Peak=1.3 Hz, FFT_BPM=78.0
Có thể dùng:

Arduino Serial Monitor.

Serial Monitor của STM32CubeIDE.

PuTTY / TeraTerm / RealTerm, v.v.

6. Cách build & nạp chương trình
Tạo project STM32F401RE trong STM32CubeIDE (hoặc dùng project có sẵn).

Thêm file main.c này vào project, đảm bảo bật:

ADC1 trên PA0.

USART2 trên PA2/PA3.

Thêm CMSIS-DSP:

Include arm_math.h.

Thêm đường dẫn CMSIS/Include và CMSIS/DSP/Include vào include path.

Link các file nguồn/binary của CMSIS-DSP cho Cortex-M4F (có FPU).

Build project, nạp xuống Nucleo-F401RE qua ST-LINK.

Mở Serial Monitor ở 115200 baud, reset board và đặt ngón tay lên Pulse Sensor để quan sát giá trị BPM & FFT_BPM.

7. Ghi chú & Tinh chỉnh
Nếu tín hiệu quá nhiễu / không lên nhịp:

Kiểm tra mass chung, dây nối, nguồn cảm biến.

Giảm MIN_PEAK_TO_PEAK nếu tín hiệu yếu.

Điều chỉnh TH_HI_PCT, TH_LO_PCT để ngưỡng Schmitt phù hợp hơn.

Nếu FFT_BPM lệch nhiều so với BPM thời gian:

Đảm bảo bạn giữ tay ổn định đủ lâu để đủ 1024 mẫu ổn định.

Có thể thu hẹp hoặc dịch dải f_min, f_max để loại nhiễu tần số khác.
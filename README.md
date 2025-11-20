Dưới đây là nội dung `README.md` bạn có thể copy dán thẳng vào file trong project rồi push lên GitHub.

---

````markdown
# STM32 DSP Heart-Rate Monitor (Nucleo-F401RE + Pulse Sensor)

DSP project on **STM32F401RE Nucleo** for heart-rate acquisition and analysis using a Pulse Sensor.  
The firmware performs **ADC sampling, DC removal, low-pass filtering, adaptive peak detection, BPM calculation, and DFT-based frequency analysis (manual FFT)**.  
Results are printed over **USART2 @ 115200 baud** and can be viewed in the Arduino Serial Monitor / any terminal.

---

## 1. Hardware

- Board: **STM32F401RE Nucleo**
- Sensor: **Pulse Sensor** (analog output)
- Connections:
  - Pulse Sensor signal → **PA0** (ADC1_IN0)
  - On-board LED LD2 → **PA5** (heart-beat blink)
  - USART2:
    - **PA2** – TX (connect to USB-UART if needed, on Nucleo already wired to ST-Link)
    - **PA3** – RX (unused in this demo)

Power the Nucleo through the USB ST-Link connector.

---

## 2. Main Features

1. **Continuous ADC sampling**
   - ADC1 reads PA0 in continuous mode (12-bit, 0…4095).

2. **DC removal (baseline wander removal)**
   - A 1-second **moving average** is computed on the raw ADC signal.
   - DC-free signal:  
     \[
     x_{\text{dc}}[n] = \text{raw}[n] - \text{mean}_{1\,\text{s}}[n]
     \]

3. **Low-pass IIR filter (smoothing)**
   - First-order IIR used as a simple low-pass filter:
     \[
     y[n] = 0.85 \cdot y[n-1] + 0.15 \cdot x_{\text{dc}}[n]
     \]
   - This reduces high-frequency noise and makes the peaks cleaner.

4. **Adaptive peak detection + Schmitt trigger**
   - A sliding **1-second window** keeps track of `min` and `max` of the filtered signal.
   - Peak-to-peak amplitude `pk2pk = max - min` is used to define dynamic thresholds:
     - High threshold:  `th_hi = min + 65% * pk2pk`
     - Low threshold:   `th_lo = min + 45% * pk2pk`
   - A two-state **state machine** is used:
     - `WAIT_RISE`: wait for signal to cross `th_hi` with enough amplitude.
     - `WAIT_FALL`: track the local maximum, then wait until signal falls below `th_lo` to **confirm a beat**.
   - A **refractory period** `REFRACTORY_MS = 330 ms` prevents double-counting.
   - Minimum peak-to-peak (`MIN_PEAK_TO_PEAK = 40`) filters out noise bursts.

5. **Time-domain BPM calculation**
   - For each accepted beat, the **RR interval** (ms) between consecutive beats is measured.
   - Only RR in the range `[350 ms, 2000 ms]` (≈ 30–171 BPM) is considered valid.
   - An average over the last 4 RR values is used:
     \[
     \text{BPM}_{\text{time}} = \frac{60000}{\text{mean\_RR}}
     \]

6. **DFT / “FFT”-based frequency analysis**
   - Filtered signal `sig` is stored into a buffer of **1024 samples** (`FFT_N = 1024`) at a logical sampling rate of **100 Hz**.
   - When the buffer is full, a **manual DFT** is computed for bins `k = 0…FFT_N/2 - 1` (real-signal half-spectrum).
   - Magnitude:
     \[
     |X(k)| = \sqrt{ \text{Re}^2 + \text{Im}^2 }
     \]
   - The spectral peak is searched **only in 0.7–3 Hz** (≈ 42–180 BPM) to reject slow drift and very high noise.
   - Frequency resolution:
     \[
     \Delta f = \frac{F_s}{N} = \frac{100}{1024} \approx 0.098 \text{ Hz}
     \]
   - FFT-based BPM:
     \[
     \text{BPM}_{\text{FFT}} = f_{\text{peak}} \times 60
     \]

7. **LED beat indicator**
   - Each accepted beat turns **LED PA5** ON for `LED_ON_MS = 30 ms`, giving a visible blink for every heart beat.

8. **No-pulse watchdog**
   - If no beat is detected for more than **2 seconds**, `BPM` is reset to 0.

---

## 3. UART Output Format

Baud rate: **115200**, 8-N-1, on **USART2** (PA2 TX).  

The firmware prints 2 loại dòng chính:

1. **Beat event (tùy chọn – có thể tắt trong code)**

   ```text
   >>> Phat hien nhip!  BPM:75
   >>> Phat hien nhip!  BPM:122  (CANH BAO >110)
````

* In ra mỗi khi phát hiện một nhịp mới.
* Nếu `BPM > 110`, thêm chuỗi cảnh báo.

2. **Tổng hợp định kỳ 2 lần/giây (mỗi vòng lặp 500 ms)**

   ```text
   -> BPM:75 FFT_Peak=1.3 Hz, FFT_BPM=78.0
   ```

   * `BPM`      : giá trị BPM trung bình trong miền thời gian.
   * `FFT_Peak` : tần số đỉnh phổ (Hz) trong dải 0.7–3 Hz.
   * `FFT_BPM`  : BPM suy ra từ đỉnh phổ.

Bạn có thể mở **Arduino Serial Monitor** hoặc bất kỳ terminal nào, chọn `115200 baud` để quan sát kết quả.

---

## 4. Algorithm Summary (Vietnamese)

Chương trình lấy mẫu tín hiệu nhịp tim từ cảm biến Pulse Sensor qua ADC1 kênh PA0 ở tần số logic 100 Hz. Tín hiệu thô trước tiên được khử thành phần DC bằng trung bình trượt trong 1 giây, sau đó đi qua bộ lọc thông thấp IIR bậc 1 (hệ số 0.85/0.15) để làm mượt. Trên tín hiệu đã lọc, chương trình duy trì một cửa sổ 1 giây để ước lượng giá trị nhỏ nhất và lớn nhất, từ đó tính biên độ peak-to-peak và sinh ra cặp ngưỡng Schmitt cao / thấp. Một state machine hai trạng thái (WAIT_RISE, WAIT_FALL) dùng các ngưỡng này để phát hiện đỉnh nhịp tim, kết hợp với thời gian trơ nhằm tránh đếm đôi. Mỗi khi một nhịp hợp lệ được chốt, khoảng RR giữa hai nhịp liên tiếp được cập nhật vào bộ đệm và dùng để tính BPM trung bình. Song song, 1024 mẫu tín hiệu đã lọc được lưu lại để thực hiện DFT thủ công, tìm đỉnh phổ trong dải tần tương ứng với nhịp tim sinh lý. Cuối cùng, chương trình in ra cả BPM theo miền thời gian và BPM theo miền tần số, đồng thời nháy LED PA5 mỗi khi phát hiện nhịp mới.

---

## 5. Build & Run

1. Open the project in **STM32CubeIDE** (or your preferred IDE using HAL drivers).
2. Select board / MCU: **STM32F401RE**.
3. Ensure clock tree is configured to **84 MHz** system clock (HSI + PLL as in `SystemClock_Config()`).
4. Build and flash the firmware to the Nucleo board.
5. Open a serial terminal at **115200 baud** on the Nucleo’s virtual COM port.
6. Place the Pulse Sensor firmly on a fingertip and observe:

   * LED LD2 blinking at each heartbeat.
   * BPM and FFT information streaming over UART.

---

## 6. File Overview

* `Core/Src/main.c` – main application: acquisition, filtering, peak detection, BPM, DFT.
* `Core/Src/system_stm32f4xx.c`, `startup_stm32f401retx.s` – system and startup files generated by STM32CubeIDE.
* `Drivers/*` – HAL and CMSIS libraries.

---

## 7. License

This project is released under the **MIT License**.
See the `LICENSE` file for details.

```

---

Nếu bạn muốn mình viết thêm bản README tiếng Việt 100% (không song ngữ) hoặc thêm hình minh họa / sơ đồ kết nối để chèn vào GitHub thì nói mình bổ sung tiếp nhé.
::contentReference[oaicite:0]{index=0}
```

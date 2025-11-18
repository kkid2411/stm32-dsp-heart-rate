# stm32-dsp-heart-rate
# STM32 DSP Heart-Rate Measurement Project

This project implements a complete heart-rate acquisition and digital signal processing (DSP) pipeline on the **STM32F401RE Nucleo** using a **Pulse Sensor**.  
The system performs real-time ADC sampling, noise reduction, peak detection, BPM calculation, and FFT/DFT frequency-domain analysis.

---

## 🔧 Hardware Requirements
- STM32F401RE Nucleo Board  
- Pulse Sensor (PPG sensor)  
- USB Cable (for programming & serial communication)  
- Jumper wires  
- Optional: Arduino IDE Serial Plotter for signal visualization  

---

## 🧠 Main Features
### **1. ADC Signal Acquisition**
- Read pulse waveform from PA0 (ADC1_IN0).  
- Continuous ADC sampling (12-bit resolution).  
- Software-decimated processing at **100 Hz**.

### **2. Noise Reduction**
- **DC removal**: 1-second moving average.  
- **Low-pass IIR filter**:  
  \[
  y[n] = 0.85y[n-1] + 0.15x[n]
  \]
- Smooths the PPG waveform and removes high-frequency noise.

### **3. Adaptive Peak Detection**
- Dynamic Schmitt trigger thresholds:
  - High threshold = min + 65%·p2p  
  - Low threshold = min + 45%·p2p  
- State machine:
  - WAIT_RISE  
  - WAIT_FALL  
- Refractory time to prevent double-counting peaks.

### **4. BPM Calculation**
- RR interval measurement (time between beats).  
- Averaging over 4 RR intervals for stable BPM.  
- Watchdog: If no beat is detected for 2 seconds → BPM resets to 0.

### **5. Frequency-Domain Analysis (DFT/FFT)**
- Collects 1024 processed samples.  
- Uses a custom DFT implementation (no CMSIS-DSP).  
- Computes magnitude spectrum and finds dominant frequency.  
- Converts peak frequency → BPM (FFT_BPM).

### **6. UART Output**
Two output formats:
- **Text** → for Serial Monitor  
- **Key:value pairs** → for Serial Plotter  
Example:
tho:1234 nen:1100 tin:-45 Min:980 Max:1350 BPMx20:1200
---

## 📁 Project Structure
Core/
├── Inc/
├── Src/
Drivers/
├── CMSIS/
├── STM32F4xx_HAL_Driver/
README.md
LICENSE
.gitignore

---

## 🚀 How to Build & Run
### **1. Open Project**
Open the folder in **STM32CubeIDE**.

### **2. Flash to MCU**
Click:
Run → Run
or
Debug → Debug

### **3. View Results**
- Open Arduino IDE → Serial Plotter → 115200 baud  
- View real-time pulse waveform and BPM  
- FFT results appear every ~5 seconds:
FFT_Peak=1.0 Hz, FFT_BPM=60.0
---

## 📊 Example Output (Serial Monitor)
Phat hien nhip! BPM:59
tho:2150 nen:2000 tin:-120 Min:1830 Max:2420 BPMx20:1180
FFT_Peak=1.0 Hz, FFT_BPM=60.2

---

## 📝 License
This project is released under the **MIT License**.

---

## 👤 Author
**kkid2411**  
Educational & laboratory STM32 DSP project.  
Feel free to fork or contribute!

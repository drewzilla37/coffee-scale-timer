# ☕️ Coffee Scale Timer

A minimal yet powerful open-source coffee brewing scale and timer — built using an Arduino Nano, OLED display, and a load cell. Designed to be responsive, intuitive, and reliable for pour-over brewing and other precise coffee-making methods.

---

## ✨ Features

- **Real-Time Weight Display**: Accurate to 0.1g under 100g, rounded to whole grams above.
- **Brewing Timer**: Start/stop/reset the timer with a single button.
- **Negative Weight Support**: Clearly shows negative values with a fixed-position minus sign.
- **Single Button Interface**:
  - Short press: start → pause → reset the timer.
  - Long press: tare the scale.
- **OLED Display**: Clean, legible layout on a 128x64 screen using [U8g2 library](https://github.com/olikraus/u8g2).

---

## 🛠️ Hardware

| Component         | Description                                      |
|------------------|--------------------------------------------------|
| Arduino Nano      | Or compatible microcontroller                    |
| HX711             | 24-bit ADC for load cell readings                |
| Load Cell         | 5kg recommended (others can work with tweaks)    |
| SSD1306 OLED      | 128x64 display (I²C)                             |
| Tactile Button    | For input (with pull-up enabled)                 |
| Power             | USB or external 5V                               |

---

## 🧠 How It Works

- Reads mass from the HX711 and load cell
- Filters and calibrates readings before display.
- Uses a finite state machine for timer control (`HIDDEN`, `RUNNING`, `PAUSED`).
- The OLED UI displays icons, a dynamic timer, and live mass with proper formatting for limited screen space.

---

## 🚀 Getting Started

### 📦 Dependencies

Install the following libraries via Arduino Library Manager:

- [`HX711`](https://github.com/bogde/HX711)
- [`U8g2`](https://github.com/olikraus/u8g2)

### 🧪 Calibration

Update the calibration factor in the code:
```cpp
massGrams = (raw - tareOffset) / 420.0;  // Adjust this to your load cell

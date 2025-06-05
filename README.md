# ☕ Coffee-Scale-Timer

> A minimalist open-source coffee scale and timer using Arduino, HX711, and a 128×64 OLED display.

---

## 🚀 Features

| Feature                        | Description                                                                 |
|-------------------------------|-----------------------------------------------------------------------------|
| 📏 Mass Measurement           | Reads weight in grams with relative taring at startup                       |
| ⏱️ Timer Logic                | Single-button interface for Start / Pause / Reset                           |
| 🎯 Precision Display          | 1 decimal below 100g, no decimal at 100g or more                            |
| ➖ Negative Weight Handling    | Shows a clean negative sign offset to the left when weight is negative     |
| 🔘 Long Press Tare            | Hold button for 1s to tare without affecting timer                          |
| 🧠 Smart UI                   | Timer and clock icon are hidden when inactive                               |
| 🧩 Modular Codebase           | Clean, well-commented, and easy to adapt                                    |

---

## 🔧 Hardware

| Component                     | Notes                                               |
|------------------------------|-----------------------------------------------------|
| Arduino Nano (or compatible) | Brain of the operation                              |
| HX711                        | Amplifies the load cell signal                     |
| Load Cell (5kg recommended)  | Measures weight                                     |
| SSD1306 OLED (128×64, I²C)   | Displays mass and timer                             |
| Push Button                  | Controls timer and taring                           |
| Jumper Wires + Breadboard    | For prototyping                                     |

### 🛠️ Load Cell Mounting Hardware

| Part                        | Quantity |
|----------------------------|----------|
| M4 socket head cap screws  | 2        |
| M5 socket head cap screws  | 2        |

---

## 📦 Libraries Used

- [`HX711`](https://github.com/bogde/HX711) — Load cell amplifier interface  
- [`U8g2`](https://github.com/olikraus/u8g2) — Graphics library for OLED displays

---

## 📋 How to Use

1. 🔌 Wire up the hardware according to your Arduino pin definitions  
2. 📥 Flash the Arduino sketch using the Arduino IDE  
3. ⚖️ On startup, the scale automatically tares  
4. 🔘 Use the button to:
   - Short press: Start → Pause → Reset & hide timer
   - Long press (1s+): Tare the scale

---

## 🧪 Example Behaviors

- Mass below 100g shows: `  23.5 g`  
- Mass at 100g or more: ` 123 g`  
- Mass below 0: shows a negative sign aligned left → `- 12.3 g`

---

## 📜 License

MIT License — Free to use, modify, and distribute.

---

### ☕ Built for precision. Designed for simplicity. Brew better.


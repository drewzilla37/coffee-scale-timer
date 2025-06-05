#include <HX711.h>
#include <U8g2lib.h>

// --- Display Setup ---
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);  // Use correct constructor for your display

// --- HX711 Setup ---
#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN   2
HX711 scale;

// --- Button Setup ---
#define BUTTON_PIN 4
#define DEBOUNCE_DELAY 50           // ms
#define LONG_PRESS_DURATION 1000    // ms
bool lastButtonState = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long buttonPressStartTime = 0;
bool longPressHandled = false;

// --- Timer State ---
enum TimerState { HIDDEN, RUNNING, PAUSED };
TimerState timerState = HIDDEN;
unsigned long timerStartMillis = 0;
unsigned long pausedMillis = 0;

// --- Mass and Tare ---
float massGrams = 0.0;
long tareOffset = 0;

// --- HX711 Sampling ---
unsigned long lastHX711Update = 0;
const unsigned int updateInterval = 1000 / 80;  // ~12.5ms

// --- Clock Icon ---
static const unsigned char image_clock_alarm_bits[] = {
  0x9e,0x3c,0xcd,0x59,0xb7,0x76,0x0b,0x68,
  0x05,0x50,0x82,0x20,0x82,0x20,0x81,0x40,
  0x83,0x60,0x41,0x40,0x22,0x20,0x12,0x20,
  0x04,0x10,0x08,0x08,0xb4,0x16,0xc2,0x21
};

static const unsigned char image_Layer_11_bits[] = {
  0x00,0x07,0x00,0x80,0x08,0x00,0x80,0x08,
  0x00,0x80,0x08,0x00,0x00,0x07,0x00,0xf0,
  0x7f,0x00,0x10,0x40,0x00,0x08,0x80,0x00,
  0x08,0x87,0x00,0x88,0x84,0x00,0x84,0x04,
  0x01,0x04,0x07,0x01,0x02,0x04,0x02,0x02,
  0x03,0x02,0x02,0x00,0x02,0x01,0x00,0x04,
  0xff,0xff,0x07
};

// --- Setup ---
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  u8g2.begin();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_gain(128);       // Default gain
  delay(500);                // Let it settle
  tareScale();               // Tare at startup
}

// --- Read and Tare Functions ---
void readLoadCell() {
  if (scale.is_ready()) {
    long raw = scale.read();
    massGrams = (raw - tareOffset) / 398.0;  // Adjust 398.0 based on your calibration
  }
}

void tareScale() {
  if (scale.is_ready()) {
    tareOffset = scale.read();  // Relative tare
  }
}

// --- Timer Update ---
void updateTimer() {
  if (timerState == RUNNING) {
    unsigned long elapsed = millis() - timerStartMillis;
    if (elapsed >= 60 * 60 * 1000UL) elapsed = 59 * 60 * 1000UL + 59999;  // Cap at 59:59
    pausedMillis = elapsed;
  }
}

// --- Button State Handling ---
void handleShortPress() {
  switch (timerState) {
    case HIDDEN:
      timerStartMillis = millis();
      timerState = RUNNING;
      break;
    case RUNNING:
      timerState = PAUSED;
      break;
    case PAUSED:
      pausedMillis = 0;
      timerState = HIDDEN;
      break;
  }
}

void handleButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        // Button just pressed
        buttonPressStartTime = millis();
        longPressHandled = false;
      } else {
        // Button just released
        unsigned long pressDuration = millis() - buttonPressStartTime;
        if (!longPressHandled && pressDuration < LONG_PRESS_DURATION) {
          handleShortPress();  // Only trigger short press if no long press occurred
        }
      }
    }

    // Handle long press while held
    if (buttonState == LOW && !longPressHandled &&
        (millis() - buttonPressStartTime) >= LONG_PRESS_DURATION) {
      tareScale();
      longPressHandled = true;
    }
  }

  lastButtonState = reading;
}

// --- Display ---
void updateDisplay() {
  u8g2.clearBuffer();

  // Draw top left static icon
  u8g2.drawXBM(6, 9, 19, 17, image_Layer_11_bits);

  // Draw clock icon only when timer is active
  if (timerState != HIDDEN) {
    u8g2.drawXBM(8, 39, 15, 16, image_clock_alarm_bits);
  }

  // Mass display
  u8g2.setFont(u8g2_font_profont29_tr);
  char weightStr[8];
  float absMass = abs(massGrams);

  if (absMass < 100.0) {
    dtostrf(absMass, 5, 1, weightStr);  // width=5, 1 decimal
  } else {
    snprintf(weightStr, sizeof(weightStr), "%3d", (int)absMass);
  }

  char* weightToDraw = weightStr;
  while (*weightToDraw == ' ') {
    weightToDraw++;
  }

  // Show negative sign separately for proper alignment
  if (massGrams < 0) {
    u8g2.drawStr(21, 28, "-");  // Fixed position for minus sign
  }

  u8g2.drawStr(33, 27, weightToDraw);
  u8g2.drawStr(99, 27, "g");



  // Timer display (only if active)
  if (timerState != HIDDEN) {
    unsigned int totalSeconds = pausedMillis / 1000;
    unsigned int minutes = totalSeconds / 60;
    unsigned int seconds = totalSeconds % 60;

    char timeStr[6];
    snprintf(timeStr, sizeof(timeStr), "%02u:%02u", minutes, seconds);
    u8g2.drawStr(33, 56, timeStr);
  }

  u8g2.sendBuffer();
}

// --- Main Loop ---
void loop() {
  handleButton();

  // Update load cell at 80Hz
  if (millis() - lastHX711Update >= updateInterval) {
    lastHX711Update = millis();
    readLoadCell();
    updateTimer();
    updateDisplay();
  }
}
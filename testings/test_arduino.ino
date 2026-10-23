/*
  Arduino Uno — Baseline Test
  Verifies the board and upload path work.
  Blinks the built-in LED (D13) and prints an "alive" message to Serial.
*/

// ===== PINS =====
const uint8_t PIN_LED = LED_BUILTIN;  // D13 on Arduino Uno

// ===== TIMING =====
const uint16_t BLINK_INTERVAL_MS = 500;

// ===== GLOBALS =====
uint32_t lastBlink = 0;
bool ledState = false;

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.println(F("=== Arduino Uno Baseline Test ==="));
  Serial.println(F("Board: Arduino Uno R3"));
  Serial.println(F("Built-in LED: D13"));
  Serial.println(F("Serial baud: 9600"));
  Serial.println(F("If you see this, the board and upload path work."));
  Serial.println();
}

// ===== LOOP =====
void loop() {
  uint32_t now = millis();

  if (now - lastBlink >= BLINK_INTERVAL_MS) {
    lastBlink = now;
    ledState = !ledState;
    digitalWrite(PIN_LED, ledState ? HIGH : LOW);

    Serial.print(F("LED state: "));
    Serial.println(ledState ? F("ON") : F("OFF"));
  }
}

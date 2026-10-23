/*
  Capacitive Proximity Sensor — Test (Metal Detection)
  Reads digital output from a capacitive proximity sensor and prints state changes.
  Sensor output -> D4 (INPUT_PULLUP)
  Note: Many 3-wire capacitive/inductive proximity sensors are NPN (output pulls
  to GND when triggered). With INPUT_PULLUP, a triggered sensor reads LOW.
  If your sensor is PNP (output pulls to VCC when triggered), change INPUT_PULLUP
  to INPUT and connect a pull-down resistor, or invert the logic below.
*/

// ===== PINS =====
const uint8_t PIN_CAPACITIVE = 4;

// ===== DEBOUNCE =====
const uint16_t DEBOUNCE_MS = 50;

// ===== GLOBALS =====
uint32_t lastDebounceTime = 0;
bool lastSensorState = false;
bool stableSensorState = false;

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  pinMode(PIN_CAPACITIVE, INPUT_PULLUP);

  // Initialize state from current pin reading
  bool initial = digitalRead(PIN_CAPACITIVE) == LOW;
  lastSensorState = initial;
  stableSensorState = initial;

  Serial.println(F("=== Capacitive Proximity Sensor Test ==="));
  Serial.print(F("Sensor pin: D4 (INPUT_PULLUP)"));
  Serial.println();
  Serial.println(F("Bring a metal object near the sensor."));
  Serial.println(F("State changes will print below."));
  Serial.println();
}

// ===== LOOP =====
void loop() {
  uint32_t now = millis();

  // Read raw pin: LOW = triggered (metal present), HIGH = not triggered
  bool rawTriggered = (digitalRead(PIN_CAPACITIVE) == LOW);

  // Debounce: only accept a change after DEBOUNCE_MS of stable reading
  if (rawTriggered != lastSensorState) {
    lastDebounceTime = now;
    lastSensorState = rawTriggered;
  }

  if (now - lastDebounceTime >= DEBOUNCE_MS) {
    if (rawTriggered != stableSensorState) {
      stableSensorState = rawTriggered;

      Serial.print(F("Metal detected: "));
      Serial.println(stableSensorState ? F("YES") : F("NO"));
    }
  }
}

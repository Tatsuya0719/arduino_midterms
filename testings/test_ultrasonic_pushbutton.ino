/*
  Ultrasonic Sensor HC-SR04 + Pushbutton — Combined Test
  Reads distance (cm) and pushbutton state, prints both to Serial.
  Ultrasonic: Trig D2, Echo D3
  Pushbutton: D4 (INPUT_PULLUP), pressed = LOW = metal detected
*/

// ===== PINS =====
const uint8_t PIN_ULTRASONIC_TRIG = 2;
const uint8_t PIN_ULTRASONIC_ECHO = 3;
const uint8_t PIN_PUSHBUTTON = 4;

// ===== TIMING =====
const uint16_t READ_INTERVAL_MS = 200;
const uint32_t PULSE_TIMEOUT_US = 30000;  // 30 ms timeout for pulseIn
const uint16_t DEBOUNCE_MS = 50;

// ===== THRESHOLDS =====
const uint16_t DETECTION_DISTANCE_CM = 15;

// ===== GLOBALS =====
uint32_t lastRead = 0;
uint32_t lastDebounceTime = 0;
bool lastButtonState = false;
bool stableButtonState = false;
bool buttonToggled = false;  // latch state: press once to toggle ON/OFF

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
  pinMode(PIN_ULTRASONIC_ECHO, INPUT);
  pinMode(PIN_PUSHBUTTON, INPUT_PULLUP);

  // Initialize button state
  bool initial = (digitalRead(PIN_PUSHBUTTON) == LOW);
  lastButtonState = initial;
  stableButtonState = initial;

  Serial.println(F("=== Ultrasonic + Pushbutton Test ==="));
  Serial.print(F("Ultrasonic Trig: D2  Echo: D3"));
  Serial.println();
  Serial.print(F("Pushbutton: D4 (INPUT_PULLUP)"));
  Serial.println();
  Serial.print(F("Detection distance: "));
  Serial.print(DETECTION_DISTANCE_CM);
  Serial.println(F(" cm"));
  Serial.println(F("Place an object <15 cm from sensor, and/or press the button."));
  Serial.println();
}

// ===== HELPERS =====
uint16_t readUltrasonicDistance() {
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);

  uint32_t duration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH, PULSE_TIMEOUT_US);

  if (duration == 0) {
    return 0xFFFF;
  }

  return static_cast<uint16_t>(duration / 58);
}

// ===== LOOP =====
void loop() {
  uint32_t now = millis();

  // Read pushbutton every loop (not gated by ultrasonic interval)
  bool rawPressed = (digitalRead(PIN_PUSHBUTTON) == LOW);
  if (rawPressed != lastButtonState) {
    lastDebounceTime = now;
    lastButtonState = rawPressed;
  }

  // Detect press edge (button just went from released to pressed)
  if (now - lastDebounceTime >= DEBOUNCE_MS && rawPressed && !stableButtonState) {
    buttonToggled = !buttonToggled;       // flip latch state
    stableButtonState = true;              // mark as processed
    Serial.print(F("Button toggle -> "));
    Serial.println(buttonToggled ? F("ON (metal simulated)") : F("OFF"));
  }

  // Reset stable state when button is released
  if (!rawPressed) {
    stableButtonState = false;
  }

  // Read ultrasonic distance every 200 ms
  if (now - lastRead >= READ_INTERVAL_MS) {
    lastRead = now;

    uint16_t distance = readUltrasonicDistance();
    bool objectNear = (distance != 0xFFFF && distance <= DETECTION_DISTANCE_CM);

    // Print results
    Serial.print(F("Distance: "));
    if (distance == 0xFFFF) {
      Serial.print(F("out of range"));
    } else {
      Serial.print(distance);
      Serial.print(F(" cm"));
    }
    Serial.print(F("  | Object near: "));
    Serial.print(objectNear ? F("YES") : F("NO"));
    Serial.print(F("  | Button raw: "));
    Serial.print(rawPressed ? F("PRESSED") : F("released"));
    Serial.print(F("  | Button toggle (metal): "));
    Serial.println(buttonToggled ? F("ON") : F("OFF"));
  }
}

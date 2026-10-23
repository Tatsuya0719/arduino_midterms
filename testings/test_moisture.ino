/*
  Soil Moisture Sensor — Test
  Reads analog moisture value and prints raw reading + wet/dry classification.
  Sensor A0 pin -> A0
*/

// ===== PINS =====
const uint8_t PIN_MOISTURE = A0;

// ===== THRESHOLDS =====
// NOTE: Your module reads HIGH when dry, LOW when wet (inverted logic).
// Dry probe: ~1023   | Wet probe: ~500-600
const uint16_t MOISTURE_THRESHOLD = 700;  // <=700 = Wet, >700 = Dry

// ===== TIMING =====
const uint16_t READ_INTERVAL_MS = 200;

// ===== GLOBALS =====
uint32_t lastRead = 0;

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  pinMode(PIN_MOISTURE, INPUT);

  Serial.println(F("=== Soil Moisture Sensor Test ==="));
  Serial.print(F("Sensor pin: A0"));
  Serial.println();
  Serial.print(F("Moisture threshold: "));
  Serial.print(MOISTURE_THRESHOLD);
  Serial.println(F("  (>600 = Wet, <=600 = Dry)"));
  Serial.println(F("Insert the sensor probes into soil. Readings will print below."));
  Serial.println();
}

// ===== LOOP =====
void loop() {
  uint32_t now = millis();

  if (now - lastRead >= READ_INTERVAL_MS) {
    lastRead = now;

    uint16_t rawValue = analogRead(PIN_MOISTURE);
    // Inverted: rawValue <= THRESHOLD means wet
    Serial.print(F("Moisture raw: "));
    Serial.print(rawValue);
    Serial.print(F("  | Classification: "));
    Serial.println(rawValue <= MOISTURE_THRESHOLD ? F("Wet") : F("Dry"));
  }
}

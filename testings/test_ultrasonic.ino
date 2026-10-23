/*
  Ultrasonic Sensor HC-SR04 — Test
  Reads distance in cm and prints to Serial every 200 ms.
  Trig -> D2, Echo -> D3
*/

#include <Arduino.h>

// ===== PINS =====
const uint8_t PIN_ULTRASONIC_TRIG = 2;
const uint8_t PIN_ULTRASONIC_ECHO = 3;

// ===== TIMING =====
const uint16_t READ_INTERVAL_MS = 200;
const uint32_t PULSE_TIMEOUT_US = 30000;  // 30 ms timeout for pulseIn

// ===== GLOBALS =====
uint32_t lastRead = 0;

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
  pinMode(PIN_ULTRASONIC_ECHO, INPUT);

  Serial.println(F("=== Ultrasonic Sensor HC-SR04 Test ==="));
  Serial.print(F("Trig pin: "));
  Serial.println(PIN_ULTRASONIC_TRIG);
  Serial.print(F("Echo pin: "));
  Serial.println(PIN_ULTRASONIC_ECHO);
  Serial.println(F("Point an object at the sensor. Distance will print below."));
  Serial.println();
}

// ===== HELPERS =====
uint16_t readUltrasonicDistance() {
  // Send a 10 us trigger pulse
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);

  // Measure echo pulse width
  uint32_t duration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH, PULSE_TIMEOUT_US);

  // 0xFFFF indicates a timeout / no echo received
  if (duration == 0) {
    return 0xFFFF;
  }

  // Convert microseconds to centimeters (speed of sound ~ 343 m/s)
  return static_cast<uint16_t>(duration / 58);
}

// ===== LOOP =====
void loop() {
  uint32_t now = millis();

  if (now - lastRead >= READ_INTERVAL_MS) {
    lastRead = now;

    uint16_t distance = readUltrasonicDistance();

    Serial.print(F("Distance: "));
    if (distance == 0xFFFF) {
      Serial.println(F("out of range"));
    } else {
      Serial.print(distance);
      Serial.println(F(" cm"));
    }
  }
}

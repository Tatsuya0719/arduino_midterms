/*
  Servo Motors — Test (Continuous Rotation / SG90)
  Tests speed and direction control using writeMicroseconds().
  Chute servo -> D5, Trapdoor servo -> D6

  For continuous rotation servos:
    ~1500 us = stop
    <1500 us = rotate one direction
    >1500 us = rotate other direction
*/

#include <Servo.h>

// ===== PINS =====
const uint8_t PIN_CHUTE_SERVO = 5;
const uint8_t PIN_DOOR_SERVO = 6;

// ===== MICROSECONDS (pulse width) =====
const uint16_t SERVO_STOP = 1500;   // stopped / neutral
const uint16_t SERVO_SLOW = 1400;   // slow rotation one way
const uint16_t SERVO_MED  = 1300;   // medium rotation one way
const uint16_t SERVO_FULL = 1200;   // full speed one way
const uint16_t SERVO_REV  = 1600;   // slow rotation other way

// ===== TIMING =====
const uint16_t SERVO_MOVE_DELAY = 3000;  // ms to run each test
const uint16_t SERVO_PAUSE      = 1000;  // ms pause between tests

// ===== GLOBALS =====
Servo chuteServo;
Servo doorServo;

// ===== SETUP =====
void setup() {
  Serial.begin(9600);

  chuteServo.attach(PIN_CHUTE_SERVO);
  doorServo.attach(PIN_DOOR_SERVO);

  // Initialize both to stopped
  chuteServo.writeMicroseconds(SERVO_STOP);
  doorServo.writeMicroseconds(SERVO_STOP);

  Serial.println(F("=== Servo Motors Test (Continuous Rotation) ==="));
  Serial.print(F("Chute servo pin: "));
  Serial.println(PIN_CHUTE_SERVO);
  Serial.print(F("Trapdoor servo pin: "));
  Serial.println(PIN_DOOR_SERVO);
  Serial.println(F("Testing stop, slow, medium, full, and reverse."));
  Serial.println();
}

// ===== HELPERS =====
void testChute(uint16_t us, const __FlashStringHelper* label) {
  Serial.print(F("Chute -> "));
  Serial.print(label);
  Serial.print(F(" ("));
  Serial.print(us);
  Serial.println(F(" us)"));
  chuteServo.writeMicroseconds(us);
}

void testDoor(uint16_t us, const __FlashStringHelper* label) {
  Serial.print(F("Trapdoor -> "));
  Serial.print(label);
  Serial.print(F(" ("));
  Serial.print(us);
  Serial.println(F(" us)"));
  doorServo.writeMicroseconds(us);
}

// ===== LOOP =====
void loop() {
  // Chute servo tests
  testChute(SERVO_STOP, F("STOP"));
  delay(SERVO_MOVE_DELAY);

  testChute(SERVO_SLOW, F("SLOW"));
  delay(SERVO_MOVE_DELAY);

  testChute(SERVO_MED, F("MEDIUM"));
  delay(SERVO_MOVE_DELAY);

  testChute(SERVO_FULL, F("FULL"));
  delay(SERVO_MOVE_DELAY);

  testChute(SERVO_REV, F("REVERSE"));
  delay(SERVO_MOVE_DELAY);

  testChute(SERVO_STOP, F("STOP"));
  delay(SERVO_PAUSE);

  // Trapdoor servo tests
  testDoor(SERVO_STOP, F("STOP"));
  delay(SERVO_MOVE_DELAY);

  testDoor(SERVO_SLOW, F("SLOW"));
  delay(SERVO_MOVE_DELAY);

  testDoor(SERVO_MED, F("MEDIUM"));
  delay(SERVO_MOVE_DELAY);

  testDoor(SERVO_FULL, F("FULL"));
  delay(SERVO_MOVE_DELAY);

  testDoor(SERVO_REV, F("REVERSE"));
  delay(SERVO_MOVE_DELAY);

  testDoor(SERVO_STOP, F("STOP"));
  delay(SERVO_PAUSE);

  Serial.println(F("--- Sweep complete, repeating ---"));
  Serial.println();
  delay(2000);
}

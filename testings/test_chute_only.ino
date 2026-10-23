/*
  Chute Servo Only — Continuous Rotation Test (Symmetric Timing)
  Dry -> Wet -> Dry -> Metallic -> Dry (loop)
  Chute servo -> D5

  Uses symmetric timing: rotate out for X ms, rotate back for same X ms.
  Tune ROTATE_TO_WET_MS and ROTATE_TO_METALLIC_MS until the chute
  physically reaches each bin from Dry.
*/

#include <Servo.h>

// ===== PINS =====
const uint8_t PIN_CHUTE_SERVO = 5;

// ===== MICROSECONDS =====
const uint16_t SERVO_STOP = 1500;   // neutral / dry (stopped)
const uint16_t SERVO_ROTATE_WET = 1300;   // speed toward wet
const uint16_t SERVO_ROTATE_METALLIC = 1700; // speed toward metallic

// ===== TIMING (tune these) =====
const uint16_t ROTATE_TO_WET_MS = 1500;      // ms dry -> wet
const uint16_t ROTATE_TO_METALLIC_MS = 1500; // ms dry -> metallic
const uint16_t HOLD_AT_POSITION_MS = 2000;   // ms to hold at each bin

// ===== GLOBALS =====
Servo chuteServo;

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  chuteServo.attach(PIN_CHUTE_SERVO);
  chuteServo.writeMicroseconds(SERVO_STOP);

  Serial.println(F("=== Chute Servo Only Test ==="));
  Serial.print(F("Chute servo pin: "));
  Serial.println(PIN_CHUTE_SERVO);
  Serial.println(F("Sequence: Dry -> Wet -> Dry -> Metallic -> Dry (repeat)"));
  Serial.println(F("Symmetric timing: rotate out X ms, rotate back X ms."));
  Serial.println();
}

// ===== HELPERS =====
void stopChute() {
  chuteServo.writeMicroseconds(SERVO_STOP);
}

void rotateToWet() {
  Serial.println(F("Rotating to WET..."));
  chuteServo.writeMicroseconds(SERVO_ROTATE_WET);
}

void rotateToMetallic() {
  Serial.println(F("Rotating to METALLIC..."));
  chuteServo.writeMicroseconds(SERVO_ROTATE_METALLIC);
}

void returnToDry() {
  Serial.println(F("Returning to DRY..."));
  chuteServo.writeMicroseconds(SERVO_STOP);
}

// ===== LOOP =====
void loop() {
  // --- Dry ---
  Serial.println(F("At DRY position"));
  stopChute();
  delay(HOLD_AT_POSITION_MS);

  // --- Dry -> Wet ---
  rotateToWet();
  delay(ROTATE_TO_WET_MS);
  stopChute();
  Serial.println(F("At WET position"));
  delay(HOLD_AT_POSITION_MS);

  // --- Wet -> Dry (same time back) ---
  Serial.println(F("Rotating back to DRY from WET..."));
  chuteServo.writeMicroseconds(SERVO_ROTATE_WET == 1300 ? 1700 : 1300); // opposite direction
  delay(ROTATE_TO_WET_MS);
  stopChute();
  Serial.println(F("Back at DRY position"));
  delay(HOLD_AT_POSITION_MS);

  // --- Dry -> Metallic ---
  rotateToMetallic();
  delay(ROTATE_TO_METALLIC_MS);
  stopChute();
  Serial.println(F("At METALLIC position"));
  delay(HOLD_AT_POSITION_MS);

  // --- Metallic -> Dry (same time back) ---
  Serial.println(F("Rotating back to DRY from METALLIC..."));
  chuteServo.writeMicroseconds(SERVO_ROTATE_METALLIC == 1700 ? 1300 : 1700); // opposite direction
  delay(ROTATE_TO_METALLIC_MS);
  stopChute();
  Serial.println(F("Back at DRY position"));
  delay(HOLD_AT_POSITION_MS);

  Serial.println(F("--- Cycle complete, repeating ---"));
  Serial.println();
}

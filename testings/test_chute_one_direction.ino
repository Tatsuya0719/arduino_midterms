/*
  Chute Servo — One Direction Rotation Test
  Continuous rotation servo on D5.
  Rotates in one direction only for a set time, then stops.
  Chute servo -> D5

  For continuous rotation servos:
    1500 us = stop
    <1500 us = rotate one direction (speed varies with value)
    >1500 us = rotate other direction

  Tune ROTATE_SPEED and ROTATE_DURATION_MS to reach your bin.
*/

#include <Servo.h>

const uint8_t PIN_CHUTE_SERVO = 5;

const uint16_t SERVO_STOP = 1500;
const uint16_t SERVO_ROTATE_OUT = 1300;  // toward bin
const uint16_t SERVO_ROTATE_BACK = 1700; // back to dry
const uint16_t ROTATE_OUT_MS = 1600;     // how long to rotate out
const uint16_t ROTATE_BACK_MS = 1500;    // how long to rotate back
const uint16_t HOLD_MS = 3000;           // how long to hold stopped

Servo chuteServo;

void setup() {
  Serial.begin(9600);
  chuteServo.attach(PIN_CHUTE_SERVO);
  chuteServo.writeMicroseconds(SERVO_STOP);

  Serial.println(F("=== Chute One Direction Test ==="));
  Serial.print(F("Rotate out speed: "));
  Serial.println(SERVO_ROTATE_OUT);
  Serial.print(F("Rotate back speed: "));
  Serial.println(SERVO_ROTATE_BACK);
  Serial.print(F("Rotate out duration: "));
  Serial.print(ROTATE_OUT_MS);
  Serial.println(F(" ms"));
  Serial.print(F("Rotate back duration: "));
  Serial.print(ROTATE_BACK_MS);
  Serial.println(F(" ms"));
  Serial.println();
}

void loop() {
  Serial.println(F("At DRY (stopped)"));
  chuteServo.writeMicroseconds(SERVO_STOP);
  delay(HOLD_MS);

  Serial.println(F("Rotating out to bin..."));
  chuteServo.writeMicroseconds(SERVO_ROTATE_OUT);
  delay(ROTATE_OUT_MS);
  chuteServo.writeMicroseconds(SERVO_STOP);
  Serial.println(F("Stopped at bin"));
  delay(HOLD_MS);

  Serial.println(F("Rotating back to DRY..."));
  chuteServo.writeMicroseconds(SERVO_ROTATE_BACK);
  delay(ROTATE_BACK_MS);
  chuteServo.writeMicroseconds(SERVO_STOP);
  Serial.println(F("Back at DRY"));
  delay(HOLD_MS);

  Serial.println(F("--- Cycle complete ---"));
  Serial.println();
}

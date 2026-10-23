/*
  Chute Servo Timing Test — Fixed Pair
  Continuous rotation servo on D5.

  Fixed sequence:
    Dry -> rotate out 1600 ms -> stop -> rotate back 1500 ms -> stop at Dry

  Repeat indefinitely.
*/

#include <Servo.h>

const uint8_t PIN_CHUTE_SERVO = 5;

const uint16_t SERVO_STOP = 1500;
const uint16_t SERVO_ROTATE_OUT = 1300;
const uint16_t SERVO_ROTATE_BACK = 1700;

const uint16_t ROTATE_OUT_MS = 1600;
const uint16_t ROTATE_BACK_MS = 1500;
const uint16_t HOLD_MS = 3000;

Servo chuteServo;

void setup() {
  Serial.begin(9600);
  chuteServo.attach(PIN_CHUTE_SERVO);
  chuteServo.writeMicroseconds(SERVO_STOP);

  Serial.println(F("=== Chute Timing Test: Out 1600 ms / Back 1500 ms ==="));
  Serial.println();
}

void loop() {
  Serial.println(F("At DRY"));
  chuteServo.writeMicroseconds(SERVO_STOP);
  delay(HOLD_MS);

  Serial.println(F("Rotating out for 1600 ms..."));
  chuteServo.writeMicroseconds(SERVO_ROTATE_OUT);
  delay(ROTATE_OUT_MS);
  chuteServo.writeMicroseconds(SERVO_STOP);
  Serial.println(F("Stopped at bin position"));
  delay(HOLD_MS);

  Serial.println(F("Rotating back to DRY for 1500 ms..."));
  chuteServo.writeMicroseconds(SERVO_ROTATE_BACK);
  delay(ROTATE_BACK_MS);
  chuteServo.writeMicroseconds(SERVO_STOP);
  Serial.println(F("Back at DRY"));
  delay(HOLD_MS);

  Serial.println(F("--- Cycle complete ---"));
  Serial.println();
}

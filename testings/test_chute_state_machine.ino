/*
  Chute Servo — Position State Machine Test
  Continuous rotation servo on D5.

  Positions (circular): DRY=0, WET=1, METALLIC=2
  Forward rotation: DRY -> WET -> METALLIC -> DRY

  Tracks current position in a variable. Each step rotates for STEP_DURATION_MS.
  You can call rotateSteps(0, 1, or 2) from any position.
*/

#include <Servo.h>

// ===== PINS =====
const uint8_t PIN_CHUTE_SERVO = 5;

// ===== MICROSECONDS =====
const uint16_t SERVO_STOP = 1500;
const uint16_t SERVO_ROTATE_FORWARD = 1300;  // DRY->WET->METALLIC->DRY

// ===== TIMING =====
const uint16_t STEP_DURATION_MS = 1700;   // time for ONE step (tune this!)
const uint16_t HOLD_MS = 3000;            // hold at each position

// ===== POSITION STATES =====
const uint8_t POS_DRY = 0;
const uint8_t POS_WET = 1;
const uint8_t POS_METALLIC = 2;

const __FlashStringHelper* posName(uint8_t p) {
  switch (p) {
    case POS_DRY:     return F("DRY");
    case POS_WET:     return F("WET");
    case POS_METALLIC:return F("METALLIC");
    default:          return F("UNKNOWN");
  }
}

// ===== GLOBALS =====
Servo chuteServo;
uint8_t currentPosition = POS_DRY;  // track position in a variable

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  chuteServo.attach(PIN_CHUTE_SERVO);
  chuteServo.writeMicroseconds(SERVO_STOP);

  Serial.println(F("=== Chute Position State Machine ==="));
  Serial.print(F("Start position: "));
  Serial.println(posName(currentPosition));
  Serial.println(F("Rotate N steps (0, 1, or 2), update position, rest, repeat."));
  Serial.println();
}

// ===== HELPERS =====
void stopChute() {
  chuteServo.writeMicroseconds(SERVO_STOP);
}

void rotateForward() {
  chuteServo.writeMicroseconds(SERVO_ROTATE_FORWARD);
}

// Rotate N steps forward from current position.
// Each step rotates individually for STEP_DURATION_MS with a brief pause between.
// Updates currentPosition after each step.
void rotateSteps(uint8_t steps) {
  if (steps == 0) {
    Serial.println(F("Rotating 0 steps (stay)."));
    return;
  }

  Serial.print(F("Rotating "));
  Serial.print(steps);
  Serial.println(F(" step(s) forward (one at a time)..."));

  for (uint8_t s = 0; s < steps; s++) {
    rotateForward();
    delay(STEP_DURATION_MS);
    stopChute();
    delay(200);  // brief pause between steps to reset

    // Update position after each step
    currentPosition = (currentPosition + 1) % 3;
    Serial.print(F("  Step "));
    Serial.print(s + 1);
    Serial.print(F(" done -> "));
    Serial.println(posName(currentPosition));
  }
}

// ===== DECISION LOGIC =====
// Modify this function to decide how many steps to rotate next.
// Returns 0, 1, or 2.
uint8_t decideNextMove() {
  // EXAMPLE: cycle through 1, 2, 0 steps repeatedly
  static uint8_t patternIndex = 0;
  const uint8_t pattern[] = {1, 2, 0};
  uint8_t move = pattern[patternIndex];
  patternIndex = (patternIndex + 1) % 3;

  Serial.print(F("Decided to rotate "));
  Serial.print(move);
  Serial.println(F(" step(s)."));
  return move;
}

// ===== LOOP =====
void loop() {
  // 1. Decide how many steps to rotate
  uint8_t steps = decideNextMove();

  // 2. Rotate that many steps (updates currentPosition automatically)
  rotateSteps(steps);

  // 3. Rest at current position
  Serial.print(F("Resting at "));
  Serial.println(posName(currentPosition));
  delay(HOLD_MS);

  Serial.println(F("--- Cycle complete ---"));
  Serial.println();
}

/*
  Servo Diagnostic Test
  Tests one servo on D5 to determine if it is standard position or continuous rotation.
  Watch the serial output and observe the servo movement at each step.
*/

#include <Servo.h>

Servo s;

void setup() {
  Serial.begin(9600);
  s.attach(5);
  Serial.println(F("Servo Diagnostic Test"));
  Serial.println(F("Watch servo on D5 at each step:"));
  Serial.println();
}

void loop() {
  Serial.println(F("STEP 1: write(0) - standard servo goes to 0 degrees"));
  s.write(0);
  delay(3000);

  Serial.println(F("STEP 2: write(90) - standard servo goes to center"));
  s.write(90);
  delay(3000);

  Serial.println(F("STEP 3: write(180) - standard servo goes to 180 degrees"));
  s.write(180);
  delay(3000);

  Serial.println(F("STEP 4: writeMicroseconds(1200) - continuous rotation slow"));
  s.writeMicroseconds(1200);
  delay(3000);

  Serial.println(F("STEP 5: writeMicroseconds(1500) - stop"));
  s.writeMicroseconds(1500);
  delay(3000);

  Serial.println(F("--- Cycle complete ---"));
  Serial.println();
  delay(2000);
}

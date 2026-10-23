# Wiring Guide — Servo Motors (Chute Rotator + Trapdoor)

## Purpose
Test that both servos move and stop correctly using pulse-width control.

> **Important:** Your SG90s appear to behave like **continuous rotation servos** (spinning 360° instead of holding a position). This test uses `writeMicroseconds()` for speed/direction control instead of position angles.

## Pin Map (from `test_servos.ino`)
| Arduino Pin | Function | Direction |
|-------------|----------|-----------|
| D5 | Chute servo signal | Servo output |
| D6 | Trapdoor servo signal | Servo output |

## Wiring Table (per servo)
| Servo Wire | Color | Arduino Pin | Notes |
|------------|-------|-------------|-------|
| VCC | Red | 5V | Servo power |
| GND | Brown or Black | GND | Common ground |
| Signal | Orange or Yellow | D5 (chute) / D6 (trapdoor) | Control signal |

Connect both servos the same way, but plug the chute servo signal into D5 and the trapdoor servo signal into D6.

## Power Warning
**Two servos moving at once may draw more current than the Arduino Uno's 5V regulator can provide.** If servos jitter, reset the Arduino, or behave erratically:
- Use an external 5V power supply (4× AA batteries or a 5V UBEC) connected to servo VCC/GND.
- Connect the external supply GND to Arduino GND (common ground required).
- Only connect the signal wires (D5, D6) to the Arduino.

## How it works (Continuous Rotation)
| Pulse Width | Behavior |
|-------------|----------|
| ~1500 µs | Stopped / neutral |
| <1500 µs | Rotate one direction (slower as it approaches 1500) |
| >1500 µs | Rotate other direction (faster as it moves away from 1500) |

The test cycles through: STOP → SLOW → MEDIUM → FULL → REVERSE → STOP for each servo.

## How to Run
1. Wire both servos per the table above.
2. Open `test_servos.ino` in Arduino IDE.
3. Select **Tools → Board: Arduino Uno R3** and the correct port.
4. Upload the sketch.
5. Open **Serial Monitor** at 9600 baud.
6. Each servo will cycle through stop, slow, medium, full, reverse, and stop — 3 seconds per step.

## Troubleshooting
| Problem | Likely Cause | Fix |
|---------|--------------|-----|
| Servo spins 360 continuously | Continuous rotation servo | This is expected; use writeMicroseconds to control speed/direction |
| Servo jitters / doesn't move | Insufficient power | Use external 5V supply for servos; common GND |
| Servo moves to wrong direction | Signal wire mix-up | Verify signal wire on correct pin (D5 vs D6) |
| Arduino resets when servo moves | Power dip from servo | External power supply for servos |
| Servo buzzes continuously | Mechanical binding | Ensure nothing blocks servo movement |

## Notes
- Standard SG90s are position servos (0–180°), but some clones are continuous rotation. This test assumes continuous rotation behavior.
- If your servos are actually standard position servos, use `write(angle)` instead of `writeMicroseconds()` — see the original `waste_segregator.ino` for position values.
- The `Servo.h` library is included with Arduino IDE; no extra installation needed.

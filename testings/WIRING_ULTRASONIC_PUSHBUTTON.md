# Wiring Guide — Ultrasonic Sensor + Pushbutton (Combined Test)

## Purpose
Test the HC-SR04 ultrasonic sensor and pushbutton together. The ultrasonic detects an object within 15 cm; the pushbutton simulates metal detection. Both run on Arduino 5V — no external supply needed.

## Pin Map (from `test_ultrasonic_pushbutton.ino`)
| Arduino Pin | Component | Direction |
|-------------|-----------|-----------|
| D2 | Ultrasonic Trig | OUTPUT |
| D3 | Ultrasonic Echo | INPUT |
| D4 | Pushbutton | INPUT_PULLUP |

## Wiring Table — Ultrasonic HC-SR04
| Sensor Pin | Arduino Pin | Notes |
|------------|-------------|-------|
| VCC | 5V | Sensor power |
| GND | GND | Common ground |
| Trig | D2 | Trigger pulse output |
| Echo | D3 | Echo pulse input |

## Wiring Table — Pushbutton
| Pushbutton Leg | Arduino Pin | Notes |
|----------------|-------------|-------|
| Leg 1 (top-left) | D4 | Signal input |
| Leg 2 (bottom-left) | GND | Connects D4 to GND when pressed |
| Leg 3 (top-right) | — | Not connected |
| Leg 4 (bottom-right) | — | Not connected |

## Combined Connections Summary
| Arduino Pin | Connects To |
|-------------|-------------|
| 5V | Ultrasonic VCC |
| GND | Ultrasonic GND + Pushbutton leg 2 |
| D2 | Ultrasonic Trig |
| D3 | Ultrasonic Echo |
| D4 | Pushbutton leg 1 |

## How it works
- **Ultrasonic:** Measures distance in cm. Object within 15 cm = detected.
- **Pushbutton:** `INPUT_PULLUP` reads HIGH normally, LOW when pressed. Pressed = "metal detected."
- The sketch prints both readings to Serial every 200 ms.

## How to Run
1. Wire both components per the tables above.
2. Open `test_ultrasonic_pushbutton.ino` in Arduino IDE.
3. Select **Tools → Board: Arduino Uno R3** and the correct port.
4. Upload the sketch.
5. Open **Serial Monitor** at 9600 baud.
6. Place an object <15 cm from the ultrasonic sensor → `Object near: YES`.
7. Press the pushbutton → `Button pressed (metal): YES`.

## Troubleshooting
| Problem | Likely Cause | Fix |
|---------|--------------|-----|
| Distance always "out of range" | Trig/Echo swapped or loose wiring | Verify D2=Trig, D3=Echo; check VCC/GND |
| Button always "YES" | D4 shorted to GND or button stuck | Check wiring; ensure one leg to D4, other to GND |
| Button always "NO" | Button not connected to D4/GND | Verify both legs are wired correctly |
| Erratic readings | Loose jumper wires | Secure all connections; use a breadboard |

## Notes
- This is a simulation setup. For real metal detection with the LJ12A3 later, the pushbutton on D4 can be replaced with the sensor output on the same pin — code changes are minimal.
- All components share Arduino GND; do not connect ultrasonic VCC to anything other than 5V.

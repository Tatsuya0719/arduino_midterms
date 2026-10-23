# Wiring Guide — Ultrasonic Sensor HC-SR04

## Purpose
Test that the HC-SR04 detects objects and reports distance in centimeters.

## Pin Map (from `test_ultrasonic.ino`)
| Arduino Pin | Function | Direction |
|-------------|----------|-----------|
| D2 | Trig | OUTPUT |
| D3 | Echo | INPUT |

## Wiring Table
| HC-SR04 Pin | Arduino Pin | Notes |
|-------------|-------------|-------|
| VCC | 5V | Sensor power |
| GND | GND | Common ground |
| Trig | D2 | Trigger pulse output from Arduino |
| Echo | D3 | Echo pulse input to Arduino |

No breadboard is strictly required; you can wire the 4 pins directly with jumper wires.

## How to Run
1. Open `test_ultrasonic.ino` in Arduino IDE.
2. Select **Tools → Board: Arduino Uno R3** and the correct port.
3. Upload the sketch.
4. Open **Serial Monitor** at 9600 baud.
5. Place your hand or an object in front of the sensor.
6. You should see distance readings in cm (e.g., `Distance: 12 cm`) or `out of range` when nothing is near.

## Troubleshooting
| Problem | Likely Cause | Fix |
|---------|--------------|-----|
| Always "out of range" | Wiring / power issue | Check VCC/GND and Trig/Echo connections |
| Erratic readings | Loose jumper wires | Secure connections; try a breadboard |
| Reading stuck at one value | Sensor not receiving echo | Ensure nothing blocks the sensor face; try a different surface |

## Notes
- The sensor needs a clear line of sight; soft or angled surfaces may not reflect ultrasound well.
- Maximum reliable range is about 200–400 cm depending on conditions.

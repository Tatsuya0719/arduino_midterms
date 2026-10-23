# Wiring Guide — Capacitive Proximity Sensor (Metal Detection)

## Purpose
Test that the capacitive proximity sensor detects metal objects and reports state changes.

> **Capacitive vs Inductive:** Capacitive proximity sensors detect both metal and non-metal (wood, plastic, liquid). Inductive proximity sensors detect metal only. For metal-item detection, an **inductive** sensor is typically preferred, but a capacitive sensor will also work for metal. The wiring below assumes a standard 3-wire proximity sensor.

## Pin Map (from `test_capacitive.ino`)
| Arduino Pin | Function | Direction |
|-------------|----------|-----------|
| D4 | Sensor output | INPUT_PULLUP |

## Wiring Table (3-wire NPN sensor — most common)
| Sensor Wire | Color | Arduino Pin | Notes |
|-------------|-------|-------------|-------|
| VCC / + | Brown or Red | 5V | Sensor power |
| GND / – | Blue or Black | GND | Common ground |
| Output / Signal | Black or White | D4 | Connects to D4; INPUT_PULLUP reads LOW when triggered |

### NPN sensor behavior (with INPUT_PULLUP)
- No metal near sensor → output OFF → D4 reads HIGH → "Metal detected: NO"
- Metal near sensor → output ON (pulls to GND) → D4 reads LOW → "Metal detected: YES"

## Wiring Table (PNP sensor)
| Sensor Wire | Color | Arduino Pin | Notes |
|-------------|-------|-------------|-------|
| VCC / + | Brown or Red | 5V | Sensor power |
| GND / – | Blue or Black | GND | Common ground |
| Output / Signal | Black or White | D4 | Connect via a 10 kΩ pull-down resistor to GND |

### PNP sensor behavior
- Output pulls to VCC when triggered → D4 reads HIGH → metal present.
- If using PNP, change `INPUT_PULLUP` to `INPUT` in `test_capacitive.ino` and add a pull-down resistor, or invert the logic.

## How to Run
1. Open `test_capacitive.ino` in Arduino IDE.
2. Select **Tools → Board: Arduino Uno R3** and the correct port.
3. Upload the sketch.
4. Open **Serial Monitor** at 9600 baud.
5. Bring a metal object (key, coin, can) near the sensor face.
6. You should see `Metal detected: YES` when near, `NO` when removed.

## Troubleshooting
| Problem | Likely Cause | Fix |
|---------|--------------|-----|
| Always "YES" | Sensor output shorted to GND | Check wiring; verify sensor model (NPN vs PNP) |
| Always "NO" | Sensor not powered / wrong logic | Check VCC/GND; if PNP sensor, invert logic or add pull-down |
| Erratic triggering | Loose wires / interference | Secure connections; keep sensor away from motors |

## Notes
- Check your sensor's datasheet to confirm NPN vs PNP output type.
- Sensing distance varies by sensor model (typically 2–8 mm for metal). Adjust placement accordingly.
- If the sensor has an adjustment potentiometer, turn it to set the desired sensing range.

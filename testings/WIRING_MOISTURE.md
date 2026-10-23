# Wiring Guide — Soil Moisture Sensor

## Purpose
Test that the soil moisture sensor reports analog values and classifies wet vs dry.

## Pin Map (from `test_moisture.ino`)
| Arduino Pin | Function | Direction |
|-------------|----------|-----------|
| A0 | Moisture sensor analog output | Analog INPUT |

## Wiring Table
| Moisture Sensor Pin | Arduino Pin | Notes |
|---------------------|-------------|-------|
| VCC | 5V | Sensor power |
| GND | GND | Common ground |
| A0 (analog output) | A0 | Analog reading; higher = wetter |

The sensor typically has two probes (fork) and a small amplifier board with 3 pins (VCC, GND, A0). Connect the amplifier board to the Arduino; the probes go into the soil.

## How to Run
1. Open `test_moisture.ino` in Arduino IDE.
2. Select **Tools → Board: Arduino Uno R3** and the correct port.
3. Upload the sketch.
4. Open **Serial Monitor** at 9600 baud.
5. You will see raw values (0–1023) and a classification:
   - `> 600` → Wet
   - `<= 600` → Dry
6. Insert the probes into dry soil → expect "Dry" (lower value).
7. Insert the probes into wet soil (or add water) → expect "Wet" (higher value).

## Troubleshooting
| Problem | Likely Cause | Fix |
|---------|--------------|-----|
| Value stuck at 0 | Sensor not powered / bad GND | Check VCC and GND connections |
| Value stuck at 1023 | Shorted probes / sensor in water | Remove sensor from liquid; dry the probes |
| Value doesn't change | Probes not in soil | Push probes fully into the soil; ensure contact |

## Notes
- Do not leave the sensor powered in wet soil long-term; the probes will corrode. For permanent installations, power the sensor only during readings.
- Threshold 600 is a starting point; calibrate by noting the raw value for your specific soil and adjusting `MOISTURE_THRESHOLD` in the sketch.

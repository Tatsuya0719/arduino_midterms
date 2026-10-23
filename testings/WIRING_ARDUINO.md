# Wiring Guide — Arduino Uno (Baseline Test)

## Purpose
Verify the Arduino Uno board and the upload path work before testing any sensors or actuators.

## What to Test
- The board powers on when connected via USB
- The sketch uploads successfully in Arduino IDE (Board: Arduino Uno R3)
- The built-in LED (D13) blinks
- Serial output appears in the Serial Monitor at 9600 baud

## Connections
| Arduino Pin | Connection | Notes |
|-------------|------------|-------|
| USB | Computer | Power + programming |
| D13 (LED_BUILTIN) | Built-in LED | Blinks on board; no external wiring needed |
| GND | — | Reference for Serial |
| TX (D1) / RX (D0) | USB-to-serial | Used by Serial.print; do not connect external devices while uploading |

## How to Run
1. Open `test_arduino.ino` in Arduino IDE.
2. Select **Tools → Board: Arduino Uno R3**.
3. Select **Tools → Port** (the COM port your Uno is on).
4. Upload the sketch.
5. Open **Serial Monitor** at 9600 baud.
6. You should see "=== Arduino Uno Baseline Test ===" and the built-in LED should blink every 500 ms.

## Troubleshooting
| Problem | Likely Cause | Fix |
|---------|--------------|-----|
| Upload fails | Wrong board/port selected | Verify Board = Arduino Uno R3, Port = correct COM port |
| No Serial output | Wrong baud rate | Set Serial Monitor to 9600 baud |
| LED doesn't blink | Bad board or cable | Try a different USB cable/port |

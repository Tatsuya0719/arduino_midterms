# Wiring Guide — Pushbutton (Metal Detection Simulation)

## Purpose
Test the pushbutton as a stand-in for metal detection. Pressing the button simulates "metal detected." This matches the original `waste_segregator.ino` design and runs entirely on Arduino 5V — no external supply needed.

## Pin Map
| Arduino Pin | Function | Direction |
|-------------|----------|-----------|
| D4 | Pushbutton signal | INPUT_PULLUP |

## Wiring Table
| Pushbutton Leg | Arduino Pin | Notes |
|----------------|-------------|-------|
| Leg 1 (top-left) | D4 | Signal input |
| Leg 2 (bottom-left) | GND | Button connects D4 to GND when pressed |
| Leg 3 (top-right) | — | Not connected |
| Leg 4 (bottom-right) | — | Not connected |

No breadboard is strictly required — two jumper wires are enough.

## How it works
- D4 is configured as `INPUT_PULLUP`, so it reads **HIGH** when the button is not pressed.
- When you press the button, D4 connects to GND and reads **LOW**.
- The sketch interprets **LOW** as "metal detected."

## How to Run
1. Open `test_capacitive.ino` (it already uses D4 with `INPUT_PULLUP` — same logic works for a pushbutton).
2. Upload to Arduino Uno R3.
3. Open Serial Monitor at 9600 baud.
4. Press the button → should print `Metal detected: YES`.
5. Release → should print `Metal detected: NO`.

## Troubleshooting
| Problem | Likely Cause | Fix |
|---------|--------------|-----|
| Always "YES" | Button leg shorted to GND, or D4 not pulled up | Check wiring; ensure D4 is `INPUT_PULLUP` |
| Always "NO" | Button not connected / wrong pin | Verify D4 and GND connections |
| Flickering when pressed | Mechanical bounce | The sketch has 50 ms debounce built in |

## Notes
- This simulates metal detection. For real non-contact metal detection with the LJ12A3, you'll need an external 6–36 V supply later — but the D4 pin and code stay the same.

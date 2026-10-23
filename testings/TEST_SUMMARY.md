# Test Summary — Component Verification

All test files and wiring guides live in the `testings/` folder. Nothing has been merged to `main` yet.

## Verified Working (tested with physical hardware)

| Component | Test Sketch | Wiring Guide | Notes |
|-----------|-------------|--------------|-------|
| Arduino Uno | `test_arduino.ino` | `WIRING_ARDUINO.md` | Blinks built-in LED + Serial output confirmed |
| Soil Moisture | `test_moisture.ino` | `WIRING_MOISTURE.md` | **Inverted logic**: dry reads ~1023, wet reads ~500–600. Threshold 700 (≤700 = Wet). |
| Servos (2× SG90) | `test_servos.ino` | `WIRING_SERVOS.md` | **Continuous rotation** confirmed. Use `writeMicroseconds()`: 1500 = stop, <1500 = one direction, >1500 = other direction. |
| Ultrasonic + Pushbutton | `test_ultrasonic_pushbutton.ino` | `WIRING_ULTRASONIC_PUSHBUTTON.md` | Pushbutton uses **toggle mode** (press once = latch ON/OFF). Ultrasonic prints distance every 200 ms. |

## Ready to Test (not yet verified with hardware)

| Component | Test Sketch | Wiring Guide | Blocker |
|-----------|-------------|--------------|---------|
| Ultrasonic (standalone) | `test_ultrasonic.ino` | `WIRING_ULTRASONIC.md` | None — same pins as combined test |
| Capacitive Proximity (LJ12A3-4-Z/BX) | `test_capacitive.ino` | `WIRING_CAPACITIVE.md` | Needs external 6–36 V supply (Brown wire). Reuses D4 pin, no code changes needed when ready. |

## Key Code Conventions Used

- Pins: `const uint8_t` named `PIN_<DESCRIPTIVE>`
- Timing/thresholds: `const uint16_t` SCREAMING_SNAKE
- Serial strings: wrapped in `F()` macro
- Non-blocking timing: `millis()` (no `delay()` in main logic where possible)
- Section headers: `// ===== SECTION NAME =====`
- Serial baud: 9600 for all tests

## Pin Assignments (consistent across all tests)

| Pin | Component | Direction |
|-----|-----------|-----------|
| D2 | Ultrasonic Trig | OUTPUT |
| D3 | Ultrasonic Echo | INPUT |
| D4 | Pushbutton / Capacitive (future) | INPUT_PULLUP |
| D5 | Chute Servo signal | Servo output |
| D6 | Trapdoor Servo signal | Servo output |
| A0 | Moisture sensor | Analog INPUT |

## Next Steps for Main Project (`waste_segregator.ino`)

When ready to integrate into the main sketch:

1. **Moisture**: Use inverted logic — `rawValue <= MOISTURE_THRESHOLD` = Wet
2. **Servos**: Use `writeMicroseconds()` instead of `write(angle)` for continuous rotation
3. **Pushbutton**: Toggle mode can replace momentary logic if latched metal detection is desired
4. **Capacitive**: Same D4 pin as pushbutton — drop-in replacement when external power is available

# Automated Waste Segregator

An Arduino Uno–based automated waste segregation system designed for Tinkercad simulation. The system detects an object via ultrasonic sensor, classifies it as **Metallic**, **Wet**, or **Dry**, then rotates a chute servo to the correct bin and opens a trapdoor to drop the item.

## Project Files

| File | Purpose |
|------|---------|
| `waste_segregator.ino` | Arduino sketch (state machine, sensing, servo control) |
| `WIRING_GUIDE.md` | Tinkercad wiring connections, pin map, simulation test procedure, troubleshooting |
| `circuit.png` | Circuit diagram reference image |
| `README.md` | This project documentation |

## How It Works

The sketch runs a finite-state machine:

```
IDLE ──► DETECTED ──► CLASSIFYING ──► ROTATING ──► OPENING ──► WAITING ──► CLOSING ──► RESETTING ──► IDLE
```

1. **IDLE** — Waiting for an object.
2. **DETECTED** — Ultrasonic sensor (HC-SR04) detects an object closer than 15 cm.
3. **CLASSIFYING** — Determines waste type:
   - **Metallic** — pushbutton (simulated metal) pressed → `INPUT_PULLUP` reads LOW
   - **Wet** — soil moisture analog reading > 600
   - **Dry** — moisture reading ≤ 600
4. **ROTATING** — Chute servo rotates to the position for the classified waste type.
5. **OPENING** — Trapdoor servo opens.
6. **WAITING** — Door stays open for 2 s to let the item fall.
7. **CLOSING** — Trapdoor closes.
8. **RESETTING** — Chute returns to home (90°), system back to IDLE.

All transitions are logged to Serial at 9600 baud.

## Components

| # | Component | Arduino Pin | Notes |
|---|-----------|-------------|-------|
| 1 | Arduino Uno R3 | — | Main controller |
| 2 | Ultrasonic HC-SR04 | Trig D2, Echo D3 | Detection < 15 cm |
| 3 | Soil Moisture Sensor | A0 | Threshold 600 (wet/dry) |
| 4 | Pushbutton | D4 (INPUT_PULLUP) | Simulates metallic detection |
| 5 | Servo: Chute Rotator | D5 | 30° metallic / 90° wet / 150° dry |
| 6 | Servo: Trapdoor | D6 | 90° closed / 0° open |
| — | Breadboard | — | Optional, for organization |
| — | 9V Battery | Vin / GND | Powers the Arduino (see Power Notes) |
| — | Power Switch | — | Optional, between battery and Arduino |

All VCC pins connect to Arduino 5V; all GND pins connect to Arduino GND.

## Key Constants (`waste_segregator.ino`)

```cpp
const uint16_t MOISTURE_THRESHOLD    = 600;   // >600 = Wet, else Dry
const uint16_t DETECTION_DISTANCE_CM = 15;    // Ultrasonic trigger distance
const uint16_t CHUTE_HOME     = 90;           // Wet / home
const uint16_t CHUTE_METALLIC = 30;           // Metallic
const uint16_t CHUTE_DRY      = 150;          // Dry
const uint16_t DOOR_CLOSED    = 90;           // Trapdoor closed
const uint16_t DOOR_OPEN      = 0;            // Trapdoor open
const uint16_t DOOR_OPEN_TIME = 2000;         // ms door stays open
const uint16_t SERVO_MOVE_DELAY = 500;        // ms between servo moves
const uint16_t ULTRASONIC_COOLDOWN = 1000;    // ms between detections
```

## Simulation Test Procedure (Tinkercad)

1. Start the simulation; open the Serial Monitor at **9600 baud**.
2. Click the ultrasonic sensor and drag the **Distance** slider to **< 15 cm**.
3. During classification:
   - **Metallic**: press & hold the pushbutton → `CLASS: METALLIC`
   - **Wet**: set moisture slider **> 600** → `CLASS: WET`
   - **Dry**: set moisture slider **< 600** → `CLASS: DRY`
4. Watch the servos: chute rotates → trapdoor opens → closes → chute returns home.
5. Serial output shows the full state-machine trace.

See `WIRING_GUIDE.md` for the full test procedure and troubleshooting table.

## Power Notes

Sensors and both servos draw from Arduino 5V/GND. This works fine in Tinkercad; real hardware may need an external 5V supply for the servos.

### Option A: 9V battery → Arduino Vin (simplest)
Connect the 9V battery to the Arduino's **Vin** and **GND** pins (via a barrel jack adapter or directly). The Arduino's onboard regulator steps 9V down to 5V, which feeds the sensors and servos through the 5V rail.

- **Pros:** Minimal wiring, single power source.
- **Cons:** 9V batteries have limited current capacity. Two servos can draw current spikes during movement, which may brown out the Arduino or cause servo jitter. OK for demos; marginal for real hardware.

### Option B: 9V battery → separate 5V buck regulator for servos (recommended for real hardware)
Connect the 9V battery to a small 5V step-down (buck) regulator. Power the servos from the regulator's 5V output, and power the Arduino separately (USB or its own supply). Tie all GNDs together.

- **Pros:** Servos get clean, dedicated current; the Arduino isn't stressed by servo spikes.
- **Cons:** Extra component (regulator), slightly more wiring.

### Common additions
- Add an **SPST power switch** between the battery and the Arduino/regulator.
- Add a **fuse** or polyfuse for short-circuit protection.

No code changes are required — the sketch runs identically regardless of power source.

## Build / Upload

1. Install the Arduino IDE.
2. Open `waste_segregator.ino`.
3. Select **Board: Arduino Uno R3** and the correct serial port.
4. Upload the sketch and open the Serial Monitor at 9600 baud.
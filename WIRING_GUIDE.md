# Tinkercad Wiring Guide — Automated Waste Segregator

## Components to Place in Tinkercad

| # | Component | Search Term in Tinkercad |
|---|-----------|--------------------------|
| 1 | Arduino Uno R3 | "Arduino Uno" |
| 2 | Ultrasonic Distance Sensor (HC-SR04) | "Ultrasonic" |
| 3 | Soil Moisture Sensor | "Soil Moisture" |
| 4 | Pushbutton | "Pushbutton" |
| 5 | Servo Motor (x2) | "Servo" |
| 6 | Breadboard (optional, for organization) | "Breadboard" |

---

## Wiring Connections

### 1. Ultrasonic Distance Sensor HC-SR04
| Sensor Pin | Arduino Pin | Notes |
|------------|-------------|-------|
| VCC | 5V | |
| GND | GND | |
| Trig | **D2** | Trigger pulse output |
| Echo | **D3** | Echo pulse input |

> **Tip**: In Tinkercad simulation, click the ultrasonic sensor and drag the "Distance" slider to simulate object distance. Set < 15 cm to trigger detection.

---

### 2. Soil Moisture Sensor (Analog Output)
| Sensor Pin | Arduino Pin | Notes |
|------------|-------------|-------|
| VCC | 5V | |
| GND | GND | |
| A0 (Analog) | **A0** | Higher value = more moisture |

> **Tip**: In Tinkercad simulation, click the sensor to adjust the "Moisture Level" slider (0–1023). Set >600 for "Wet", <600 for "Dry".

---

### 3. Pushbutton (Metallic Detection Simulator)
| Button Pin | Arduino Pin | Notes |
|------------|-------------|-------|
| Leg 1 (top-left) | **D4** | |
| Leg 2 (bottom-left) | **GND** | |
| Leg 3 (top-right) | *not connected* | |
| Leg 4 (bottom-right) | *not connected* | |

> **Why this works**: Pin D4 is `INPUT_PULLUP` in code. Button connects D4→GND when pressed. Pressed = LOW = Metallic detected.

---

### 4. Chute Rotation Servo (Servo 1)
| Servo Wire | Arduino Pin | Notes |
|------------|-------------|-------|
| Red (VCC) | 5V | |
| Brown/Black (GND) | GND | |
| Orange/Yellow (Signal) | **D5** | Controls chute angle |

**Positions in code:**
- 30° = Metallic chute
- 90° = Wet chute (home)
- 150° = Dry chute

---

### 5. Trapdoor Servo (Servo 2)
| Servo Wire | Arduino Pin | Notes |
|------------|-------------|-------|
| Red (VCC) | 5V | |
| Brown/Black (GND) | GND | |
| Orange/Yellow (Signal) | **D6** | Controls trapdoor |

**Positions in code:**
- 90° = Closed
- 0° = Open (swings downward)

---

## Power Notes

Both servos + sensors draw from Arduino 5V/GND. In Tinkercad this is fine; real hardware may need an external supply for the servos.

### Option A: 9V battery → Arduino Vin (simplest)
Connect a 9V battery to the Arduino's **Vin** and **GND** pins (via a barrel jack adapter or directly). The Arduino's onboard regulator steps 9V down to 5V, feeding the sensors and servos through the 5V rail.

| Battery | Arduino Pin |
|---------|-------------|
| 9V + | Vin |
| 9V – | GND |

- **Pros:** Minimal wiring, single power source.
- **Cons:** 9V batteries have limited current capacity. Two servos can draw current spikes during movement, which may brown out the Arduino or cause servo jitter. OK for demos; marginal for real hardware.

### Option B: 9V battery → separate 5V buck regulator for servos (recommended for real hardware)
Connect the 9V battery to a small 5V step-down (buck) regulator. Power the servos from the regulator's 5V output, and power the Arduino separately (USB or its own supply). Tie all GNDs together.

| Connection | To |
|------------|----|
| 9V + | Buck regulator VIN |
| Buck regulator 5V out | Servo VCC (both servos) |
| Buck regulator GND | Servo GND + Arduino GND |
| Arduino power | USB or separate supply |

- **Pros:** Servos get clean, dedicated current; the Arduino isn't stressed by servo spikes.
- **Cons:** Extra component (regulator), slightly more wiring.

### Common additions
- Add an **SPST power switch** between the battery and the Arduino/regulator.
- Add a **fuse** or polyfuse for short-circuit protection.

---

## Simulation Test Procedure

1. **Start Simulation** → Open Serial Monitor (9600 baud)
2. **Test Ultrasonic**: Click ultrasonic sensor → drag "Distance" slider < 15 cm → should print `[ULTRASONIC] Object detected at X cm`
3. **Test Classification** (happens automatically after detection):
   - **Metallic**: Press & hold pushbutton during classification → "CLASS: METALLIC"
   - **Wet**: Click moisture sensor → set slider >600 → "CLASS: WET"
   - **Dry**: Click moisture sensor → set slider <600 → "CLASS: DRY"
4. **Watch Servos**: Chute rotates → trapdoor opens → waits → closes → chute returns home
5. **Serial Output** shows full state machine trace

---

## Troubleshooting

| Issue | Fix |
|-------|-----|
| Servos jitter | Ensure both servos share GND with Arduino |
| Ultrasonic never triggers | In sim, click sensor and drag Distance slider < 15 cm; check Trig/Echo wiring |
| Moisture always same | Click sensor in sim and drag the moisture slider |
| Button doesn't register | Verify D4→button→GND wiring; code uses INPUT_PULLUP |
| Servo hits wrong angle | Adjust `CHUTE_METALLIC`, `CHUTE_WET`, `CHUTE_DRY` constants in code |

---

## Pin Summary

```
D2  → Ultrasonic Trig
D3  → Ultrasonic Echo
D4  → Pushbutton (to GND)
D5  → Chute Servo Signal
D6  → Trapdoor Servo Signal
A0  → Moisture Sensor A0
5V  → All VCC (Ultrasonic, Moisture, 2x Servos)
GND → All GND (Ultrasonic, Moisture, 2x Servos, Button)
```
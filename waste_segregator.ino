/*
  Automated Waste Segregator - Tinkercad Simulation
  Separates: Metallic | Wet | Dry

  Components:
  - Ultrasonic Sensor HC-SR04 (distance) -> Trig Pin 2, Echo Pin 3
  - Soil Moisture Sensor (analog)        -> Pin A0
  - Pushbutton (metal sim)               -> Pin 4 (INPUT_PULLUP)
  - Servo: Chute Rotator                 -> Pin 5
  - Servo: Trapdoor                      -> Pin 6
*/

#include <Servo.h>

// ===== PINS =====
const uint8_t PIN_ULTRASONIC_TRIG = 2;
const uint8_t PIN_ULTRASONIC_ECHO = 3;
const uint8_t PIN_MOISTURE        = A0;
const uint8_t PIN_METAL_BTN       = 4;
const uint8_t PIN_CHUTE_SERVO     = 5;
const uint8_t PIN_DOOR_SERVO      = 6;

// ===== SERVO POSITIONS =====
const uint16_t CHUTE_HOME     = 90;
const uint16_t CHUTE_METALLIC = 30;
const uint16_t CHUTE_WET      = 90;
const uint16_t CHUTE_DRY      = 150;

const uint16_t DOOR_CLOSED    = 90;
const uint16_t DOOR_OPEN      = 0;

// ===== THRESHOLDS & TIMING =====
const uint16_t MOISTURE_THRESHOLD    = 600;
const uint16_t DEBOUNCE_MS           = 50;
const uint16_t SERVO_MOVE_DELAY      = 500;
const uint16_t DOOR_OPEN_TIME        = 2000;
const uint16_t ULTRASONIC_COOLDOWN   = 1000;
const uint16_t DETECTION_DISTANCE_CM = 15;

// ===== TYPES (must be before function declarations) =====
enum class WasteType { NONE, METALLIC, WET, DRY };
enum class State { IDLE, DETECTED, CLASSIFYING, ROTATING, OPENING, WAITING, CLOSING, RESETTING };

// ===== GLOBALS =====
Servo chuteServo;
Servo doorServo;

State currentState = State::IDLE;
WasteType currentWaste = WasteType::NONE;
uint32_t lastStateChange = 0;
uint32_t lastUltrasonicTrigger = 0;
bool ultrasonicTriggered = false;

// ===== SIMULATION HELPERS =====
bool metallicToggle = false;        // Toggle mode for Tinkercad: press once = metallic, press again = normal
uint32_t lastButtonPress = 0;

// ===== FORWARD DECLARATIONS =====
void transitionTo(State newState);
void runStateMachine(uint32_t now);
uint16_t readUltrasonicDistance();
uint16_t getChutePosition(WasteType type);
const char* stateToString(State s);
void handleButtonToggle(uint32_t now);

// ===== SETUP & LOOP =====
void setup() {
  Serial.begin(9600);

  pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
  pinMode(PIN_ULTRASONIC_ECHO, INPUT);
  pinMode(PIN_METAL_BTN, INPUT_PULLUP);

  chuteServo.attach(PIN_CHUTE_SERVO);
  doorServo.attach(PIN_DOOR_SERVO);

  chuteServo.write(CHUTE_HOME);
  doorServo.write(DOOR_CLOSED);
  delay(SERVO_MOVE_DELAY);

  Serial.println(F("=== Waste Segregator Ready ==="));
  Serial.println(F("Place object in receptacle..."));
}

void loop() {
  uint32_t now = millis();
  handleButtonToggle(now);
  handleUltrasonic(now);
  runStateMachine(now);
}

// ===== ULTRASONIC HANDLING =====
void handleUltrasonic(uint32_t now) {
  uint16_t distanceCm = readUltrasonicDistance();
  bool objectDetected = (distanceCm > 0 && distanceCm < DETECTION_DISTANCE_CM);

  if (objectDetected && !ultrasonicTriggered && (now - lastUltrasonicTrigger > ULTRASONIC_COOLDOWN)) {
    ultrasonicTriggered = true;
    lastUltrasonicTrigger = now;
    Serial.print(F("[ULTRASONIC] Object detected at "));
    Serial.print(distanceCm);
    Serial.println(F(" cm"));
    transitionTo(State::DETECTED);
  } else if (!objectDetected) {
    ultrasonicTriggered = false;
  }
}

// ===== BUTTON TOGGLE (for Tinkercad simulation) =====
void handleButtonToggle(uint32_t now) {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(PIN_METAL_BTN);

  // Detect falling edge (button press)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    if (now - lastButtonPress > DEBOUNCE_MS) {
      metallicToggle = !metallicToggle;
      lastButtonPress = now;
      Serial.print(F("[TOGGLE] Metallic mode: "));
      Serial.println(metallicToggle ? F("ON") : F("OFF"));
    }
  }
  lastButtonState = currentButtonState;
}

uint16_t readUltrasonicDistance() {
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);

  uint32_t duration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH, 30000UL);
  if (duration == 0) return 0xFFFF;

  return duration / 58;
}

// ===== STATE MACHINE =====
void transitionTo(State newState) {
  currentState = newState;
  lastStateChange = millis();
  Serial.print(F("[STATE] "));
  Serial.println(stateToString(newState));
}

void runStateMachine(uint32_t now) {
  switch (currentState) {
    case State::IDLE:
      break;

    case State::DETECTED:
      if (now - lastStateChange > 300) {
        transitionTo(State::CLASSIFYING);
      }
      break;

    case State::CLASSIFYING: {
      bool isMetallic = metallicToggle;
      uint16_t moisture = analogRead(PIN_MOISTURE);

      Serial.print(F("[CLASSIFY] Metallic toggle: "));
      Serial.print(isMetallic ? F("ON") : F("OFF"));
      Serial.print(F(" | Moisture: "));
      Serial.println(moisture);

      if (isMetallic) {
        currentWaste = WasteType::METALLIC;
        Serial.println(F("-> CLASS: METALLIC"));
        metallicToggle = false;  // Auto-reset after classification
      } else if (moisture > MOISTURE_THRESHOLD) {
        currentWaste = WasteType::WET;
        Serial.println(F("-> CLASS: WET"));
      } else {
        currentWaste = WasteType::DRY;
        Serial.println(F("-> CLASS: DRY"));
      }

      transitionTo(State::ROTATING);
      break;
    }

    case State::ROTATING: {
      uint16_t targetPos = getChutePosition(currentWaste);
      chuteServo.write(targetPos);
      Serial.print(F("[SERVO] Rotating chute to "));
      Serial.println(targetPos);
      transitionTo(State::OPENING);
      break;
    }

    case State::OPENING: {
      if (now - lastStateChange > SERVO_MOVE_DELAY) {
        doorServo.write(DOOR_OPEN);
        Serial.println(F("[SERVO] Trapdoor OPEN"));
        transitionTo(State::WAITING);
      }
      break;
    }

    case State::WAITING: {
      if (now - lastStateChange > DOOR_OPEN_TIME) {
        transitionTo(State::CLOSING);
      }
      break;
    }

    case State::CLOSING: {
      if (now - lastStateChange > SERVO_MOVE_DELAY) {
        doorServo.write(DOOR_CLOSED);
        Serial.println(F("[SERVO] Trapdoor CLOSED"));
        transitionTo(State::RESETTING);
      }
      break;
    }

    case State::RESETTING: {
      if (now - lastStateChange > SERVO_MOVE_DELAY) {
        chuteServo.write(CHUTE_HOME);
        Serial.println(F("[SERVO] Chute reset to HOME"));
        currentWaste = WasteType::NONE;
        transitionTo(State::IDLE);
        Serial.println(F("=== Ready for next object ==="));
      }
      break;
    }
  }
}

// ===== HELPERS =====
uint16_t getChutePosition(WasteType type) {
  switch (type) {
    case WasteType::METALLIC: return CHUTE_METALLIC;
    case WasteType::WET:      return CHUTE_WET;
    case WasteType::DRY:      return CHUTE_DRY;
    default:                  return CHUTE_HOME;
  }
}

const char* stateToString(State s) {
  switch (s) {
    case State::IDLE:        return "IDLE";
    case State::DETECTED:    return "DETECTED";
    case State::CLASSIFYING: return "CLASSIFYING";
    case State::ROTATING:    return "ROTATING";
    case State::OPENING:     return "OPENING";
    case State::WAITING:     return "WAITING";
    case State::CLOSING:     return "CLOSING";
    case State::RESETTING:   return "RESETTING";
    default:                 return "UNKNOWN";
  }
}
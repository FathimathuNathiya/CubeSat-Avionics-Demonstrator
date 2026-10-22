/*
  =========================================================
  Arduino-Based CubeSat Monitoring and Operating-Mode Demo
  =========================================================

  Features:
  - LDR light monitoring
  - NTC thermistor monitoring
  - Green, yellow and red status LEDs
  - Buzzer for critical alerts
  - Automatic and manual operating modes
  - Servo-controlled deployable-panel demonstration
  - Serial telemetry and commands


  IMPORTANT:
  Thermistor and LDR readings are raw ADC values.
  Thresholds must be calibrated for the actual hardware.
*/

#include <Servo.h>

// 1. PIN ASSIGNMENTS

const byte LDR_PIN = A0;
const byte THERMISTOR_PIN = A1;

const byte RED_LED_PIN = 2;
const byte YELLOW_LED_PIN = 3;
const byte GREEN_LED_PIN = 4;

const byte BUZZER_PIN = 5;
const byte SERVO_PIN = 6;

// 2. SENSOR THRESHOLDS

// Provisional raw LDR ADC thresholds

const int LIGHT_MIN = 200;
const int LIGHT_MAX = 700;

// Provisional raw thermistor ADC thresholds

const int TEMP_CRITICAL_LOW = 200;
const int TEMP_WARNING_LOW = 300;
const int TEMP_WARNING_HIGH = 700;
const int TEMP_CRITICAL_HIGH = 800;

// 3. SERVO POSITIONS

const int PANEL_STOW_ANGLE = 0;
const int PANEL_DEPLOY_ANGLE = 90;

// 4. OPERATING MODES

enum OperatingMode {NORMAL, WARNING, SAFE};

OperatingMode currentMode = NORMAL;

bool automaticMode = true;
bool panelDeployed = false;

int commandedServoAngle = PANEL_STOW_ANGLE;

Servo panelServo;

// 5. SENSOR VARIABES

int lightValue = 0;
int thermistorValue = 0;

unsigned long lastTelemetryTime = 0;
const unsigned long TELEMETRY_INTERVAL = 1000;

// 6. SETUP

void setup() {
  Serial.begin(9600);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  panelServo.attach(SERVO_PIN);
  panelServo.write(PANEL_STOW_ANGLE);

  turnAllIndicatorsOff();

  Serial.println(F("# CubeSat Monitoring Demonstrator"));
  Serial.println(F("# Commands: AUTO, NORMAL, WARNING, SAFE, DEPLOY, STOW, DEPLOY <angle>, STATUS, HELP")); }

// 7. MAIN LOOP

void loop() {
  readSensors();

  processSerialCommands();

  if (automaticMode) {
    updateAutomaticMode(); }
  
  updateIndicators();

  if (millis() - lastTelemetryTime >= TELEMETRY_INTERVAL) {
    lastTelemetryTime = millis();
    printTelemetry(); }
  
  delay(50); }

// 8. SENSOR READING

void readSensors() {
  lightValue = analogRead(LDR_PIN);
  thermistorValue = analogRead(THERMISTOR_PIN); }

// 9. AUTOMATIC STATUS SELECTION

void updateAutomaticMode() {
  bool lightNormal = (lightValue >= LIGHT_MIN && lightValue <= LIGHT_MAX);

  bool temperatureCritical = (thermistorValue < TEMP_CRITICAL_LOW || thermistorValue > TEMP_CRITICAL_HIGH);

  bool temperatureNormal = (thermistorValue >= TEMP_WARNING_LOW && thermistorValue <= TEMP_WARNING_HIGH);

  if (temperatureCritical) {
    currentMode = SAFE; }

  else if (!lightNormal || !temperatureNormal) {
    currentMode = WARNING; }

  else {
    currentMode = NORMAL; } }

// 10. LED AND BUZZER CONTROL

void turnAllIndicatorsOff() {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW); }

void updateIndicators() {
  turnAllIndicatorsOff();

  switch (currentMode) {
    case NORMAL:
      digitalWrite(GREEN_LED_PIN, HIGH);
      break;

    case WARNING:
      digitalWrite(YELLOW_LED_PIN, HIGH);
      break;

    case SAFE:
      digitalWrite(RED_LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      break; } }

// 11. SERVO CONTROL

void movePanelTo(int angle) {
  angle = constrain(angle, 0, 180);

  panelServo.write(angle);

  commandedServoAngle = angle;
  panelDeployed = (angle > 0);

  Serial.print(F("# Servo commanded angle: "));
  Serial.println(commandedServoAngle); }

// 12. SERIAL COMMAND PROCESSING

void processSerialCommands() {
  if (!Serial.available()) {
    return; }

  String command = Serial.readStringUntil("\n");
  command.trim();
  command.toUpperCase();

  if (command.length() == 0) {
    return; }

  if (command == "AUTO") {
    automaticMode = true;
    Serial.println(F("# Automatic mode enabled")); }

  else if (command == "NORMAL") {
    automaticMode = false;
    currentMode = NORMAL;
    Serial.println(F("# Manual NORMAL mode")); }

  else if (command == "WARNING") {
    automaticMode = false;
    currentMode = WARNING;
    Serial.println(F("# Manual WARNING mode")); }

  else if (command == "SAFE") {
    automaticMode = false;
    currentMode = SAFE;
    Serial.println(F("# Manual SAFE mode")); }

  else if (command == "DEPLOY") {
    movePanelTo(PANEL_DEPLOY_ANGLE); }

  else if (command == "STOW") {
    movePanelTo(PANEL_STOW_ANGLE); }

  else if (command.startsWith("DEPLOY ")) {
    int angle = command.substring(7).toInt();

    if (angle >= 0 && angle <= 180) {
      movePanelTo(angle); }

    else {
      Serial.println(F("# Invalid angle: use 0 to 180")); } }

  else if (command == "STATUS") {
    printStatus(); }

  else if (command == "HELP") {
    printHelp(); }

  else {
    Serial.println(F("# Unknown command. Type HELP.")); } }

// 13. TELEMETRY

void printTelemetry() {
  Serial.println(F("# ----- CUBESAT TELEMETRY -----"));

  Serial.print(F("# Elapsed time: "));
  Serial.print(millis());
  Serial.println(F(" ms"));

  Serial.print(F("# LDR reading (ADC): "));
  Serial.println(lightValue);

  Serial.print(F("# Thermistor reading (ADC): "));
  Serial.println(thermistorValue);

  Serial.print(F("# Mode: "));
  Serial.println(getModeName());

  Serial.print(F("# Automatic mode: "));
  Serial.println(automaticMode ? F("ON") : F("OFF"));

  Serial.print(F("# Servo commanded angle: "));
  Serial.print(commandedServoAngle);
  Serial.println(F(" degrees"));

  Serial.print(F("# Panel state: "));
  Serial.println(panelDeployed ? F("DEPLOYED/PARTIAL") : F("STOWED"));

  Serial.println(F("# -----------------------------")); }

void printStatus() {
  Serial.println(F("# ----- CUBESAT STATUS -----"));

  Serial.print(F("# LDR reading (ADC): "));
  Serial.println(lightValue);

  Serial.print(F("# Thermistor reading (ADC): "));
  Serial.println(thermistorValue);

  Serial.print(F("# Mode: "));
  Serial.println(getModeName());

  Serial.print(F("# Automatic mode: "));
  Serial.println(automaticMode ? F("ON") : F("OFF"));

  Serial.print(F("# Servo commanded angle: "));
  Serial.println(commandedServoAngle);
  Serial.println(F(" degrees"));

  Serial.print(F("# Panel state: "));
  Serial.println(panelDeployed ? F("DEPLOYED/PARTIAL") : F("STOWED"));

  Serial.println(F("# --------------------------")); }

void printHelp() {
  Serial.println(F("# Available commands: AUTO, NORMAL, WARNING, SAFE, DEPLOY, STOW, DEPLOY <angle>, STATUS, HELP")); }

// 14. MODE NAME

const char* getModeName() {
  switch (currentMode) {
    case NORMAL:
      return "NORMAL";

    case WARNING:
      return "WARNING";

    case SAFE:
      return "SAFE";

    default:
      return "UNKNOWN"; } }



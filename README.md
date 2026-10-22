# CubeSat-Avionics-Demonstrator

An Arduino-based CubeSat avionics demonstrator integrating environmental sensors, onboard control logic, status indicators, serial telemetry, and servo-actuated panel deployment.

## Overview

The CubeSat Avionics Demonstrator is an educational hardware project developed to explore fundamental spacecraft avionics concepts through embedded systems programming and physical prototyping.

The demonstrator uses an Arduino microcontroller to collect sensor readings, evaluate operating conditions, communicate system status, and control a servo-operated model panel.

The project combines electronics, programming, sensor interfacing, and mechanical actuation in a small-scale spacecraft-inspired platform.

## Objectives

- Explore embedded systems concepts relevant to spacecraft avionics.
- Integrate environmental sensors with a microcontroller.
- Implement operating-mode logic and status indicators.
- Develop serial commands for monitoring and controlling the system.
- Demonstrate servo-controlled deployment and stowing of a model panel.

## Hardware Components

- Arduino Uno
- Light-dependent resistor (LDR)
- NTC thermistor
-	Servo motor
-	LEDs
-	Active buzzer
-	Resistors, breadboard, and jumper wires
-	Cardboard CubeSat model

## Implemented Features
-	Environmental sensing: Reads analogue values from the LDR and thermistor.
-	Operating-mode logic: Demonstrates NORMAL, WARNING, and SAFE operating states.
- Visual and audible indicators: Uses LEDs and a buzzer to communicate system status.
-	Serial command interface: Supports commands for system monitoring and control.
-	Model-panel actuation: Uses a servo motor to demonstrate panel deployment and stowing.
-	Serial telemetry: Displays labelled sensor readings and system information through the Arduino Serial Monitor.

## Software and Technical Skills

-	Programming: C++ (Arduino)
-	Development Environment: Arduino IDE
-	Embedded Systems: Sensor interfacing and operating-mode logic
-	Communication: Serial communication and command handling
-	Actuation and Control: Servo motor control
-	Electronics: Analogue sensor interfacing and digital output control

## Project Structure

```text
CubeSat_Avionics_Demonstrator/
│
├── README.md
│
├── src/
│   └── cubesat_avionics.py   
│ 
└── images/
     ├── cubesat_model.png
     ├── circuit_setup.png
     ├── servo_panel_deployment.png
     └── serial_monitor.png    
```

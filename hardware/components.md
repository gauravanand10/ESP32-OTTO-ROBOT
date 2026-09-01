# Hardware Components

This project is a wireless joystick-controlled robotic system built around
two ESP32 microcontrollers and a 3D-printed Otto Ninja mechanical platform.

The hardware is divided into a **transmitter unit** and a **receiver/robot
unit**.

---

## 1. Transmitter Unit

The transmitter is responsible for reading the analog joystick input,
determining the required movement direction, and sending the command to the
receiver ESP32 through Wi-Fi using UDP.

### Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 Development Board | 1 | Reads joystick input and transmits commands |
| Analog Joystick Module | 1 | Provides X-axis and Y-axis movement input |
| Perfboard / Prototype Board | 1 | Mounting and wiring the transmitter |
| Jumper Wires | As required | Electrical connections |
| USB Cable | 1 | Programming and powering the ESP32 |

### Transmitter Functions

The transmitter ESP32:

1. Reads the joystick X-axis.
2. Reads the joystick Y-axis.
3. Applies the calibrated joystick center values.
4. Applies a dead zone to eliminate small unwanted movements.
5. Determines the required direction.
6. Creates a UDP command packet.
7. Sends the command wirelessly to the receiver ESP32.

---

## 2. Receiver / Robot Unit

The receiver is mounted on the Otto Ninja robot and is responsible for
receiving the wireless movement commands and controlling the drive system.

### Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 Development Board | 1 | Receives commands and controls the robot |
| Continuous-Rotation Servo Motor | 2 | Provides differential drive |
| HC-SR04 Ultrasonic Sensor | 1 | Detects obstacles in front of the robot |
| Otto Ninja 3D-Printed Parts | 1 set | Mechanical structure of the robot |
| Battery / Power Supply | 1 | Powers the robot electronics and motors |
| Jumper Wires | As required | Electrical connections |
| Servo Mounting Hardware | As required | Mechanical servo installation |

---

## 3. Drive System

The robot uses two continuous-rotation servo motors.

Each servo controls one side of the robot.

```text
              OTTO ROBOT
          ┌───────────────┐
          │               │
          │   LEFT RIGHT  │
          │   SERVO SERVO │
          │     │     │   │
          └─────┼─────┼───┘
                │     │
              Left   Right
              Side   Side

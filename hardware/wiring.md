# Wiring

This document describes the electrical wiring of the current
ESP32-OTTO-BUGGY system.

The system consists of a wireless transmitter and a receiver mounted on
the Otto Ninja robot.

---

## 1. System Overview

```text
                    TRANSMITTER
               ┌──────────────────┐
               │  Analog Joystick │
               └────────┬─────────┘
                        │
                   VRx / VRy / SW
                        │
                        ▼
               ┌──────────────────┐
               │ ESP32 Transmitter│
               └────────┬─────────┘
                        │
                     Wi-Fi
                        │
                     UDP:4210
                        │
                        ▼
               ┌──────────────────┐
               │  ESP32 Receiver  │
               └───────┬──────┬───┘
                       │      │
                 GPIO 13    GPIO 12
                       │      │
                       ▼      ▼
                  Left Servo  Right Servo
                       │      │
                       └──┬───┘
                          ▼
                     OTTO ROBOT

                    HC-SR04
                 GPIO 26 / 25
                       │
                       ▼
                Obstacle Detection
```

---

## 2. Transmitter Wiring

The transmitter consists of an ESP32 and an analog joystick.

### Analog Joystick

Connect the joystick as follows:

```text
Joystick Pin          ESP32
──────────────────────────────
VRx                   GPIO 34
VRy                   GPIO 35
SW                    GPIO 32
VCC                   3.3V
GND                   GND
```

### Connection Diagram

```text
             ANALOG JOYSTICK
          ┌───────────────────┐
          │                   │
          │       ●           │
          │                   │
          └───────────────────┘
             │  │  │  │  │
             │  │  │  │  │
             │  │  │  │  └──── GND
             │  │  │  └─────── VCC
             │  │  └────────── SW
             │  └───────────── VRy
             └──────────────── VRx
                     │
                     ▼
                 ESP32
```

The joystick is powered from the ESP32's 3.3 V supply.

---

## 3. Receiver Wiring

The receiver ESP32 is connected to:

- Left continuous-rotation servo
- Right continuous-rotation servo
- HC-SR04 ultrasonic sensor

---

## 4. Left Servo Wiring

```text
Left Servo
────────────────────────────
Signal  → ESP32 GPIO 13
VCC     → External 5V
GND     → Common GND
```

The ESP32 GPIO 13 provides the control signal.

The servo's power should come from a suitable external power supply.

---

## 5. Right Servo Wiring

```text
Right Servo
────────────────────────────
Signal  → ESP32 GPIO 12
VCC     → External 5V
GND     → Common GND
```

The ESP32 GPIO 12 provides the control signal.

The servo's power should come from a suitable external power supply.

---

## 6. HC-SR04 Wiring

Connect the ultrasonic sensor as follows:

```text
HC-SR04
────────────────────────────
VCC     → 5V
GND     → GND
TRIG    → ESP32 GPIO 26
ECHO    → ESP32 GPIO 25
```

### Important: ECHO Voltage

A typical HC-SR04 operates at 5 V and can output approximately 5 V on its
ECHO pin.

The ESP32 GPIO operates at 3.3 V logic.

Therefore, use a suitable voltage divider or level shifter between:

```text
HC-SR04 ECHO → Voltage Divider / Level Shifter → ESP32 GPIO 25
```

Do not connect a 5 V ECHO signal directly to an ESP32 GPIO.

---

## 7. Power Wiring

The servo motors should not be powered directly from ESP32 GPIO pins.

A typical arrangement is:

```text
                 External Power Supply
                         │
                ┌────────┴────────┐
                │                 │
                ▼                 ▼
             Servo VCC        ESP32 Power
                │
          ┌─────┴─────┐
          │           │
          ▼           ▼
      Left Servo   Right Servo
```

The grounds must have a common reference:

```text
External Supply GND
        │
        ├──────── ESP32 GND
        │
        ├──────── Left Servo GND
        │
        └──────── Right Servo GND
```

Use a power supply capable of providing the required current for both
servos.

---

## 8. Complete Receiver Wiring

```text
                    ESP32 RECEIVER
                 ┌─────────────────┐
                 │                 │
 GPIO 13 ────────┤ Left Servo SIG  │
 GPIO 12 ────────┤ Right Servo SIG │
                 │                 │
 GPIO 26 ────────┤ HC-SR04 TRIG    │
 GPIO 25 ◄───────┤ HC-SR04 ECHO    │
                 │                 │
 GND ────────────┤ Common Ground   │
                 └─────────────────┘
                       │
                       │
        ┌──────────────┼──────────────┐
        │              │              │
        ▼              ▼              ▼
   Left Servo     Right Servo       HC-SR04
```

---

## 9. Complete Transmitter Wiring

```text
                  ESP32 TRANSMITTER
                ┌───────────────────┐
                │                   │
 GPIO 34 ◄──────┤ Joystick VRx      │
 GPIO 35 ◄──────┤ Joystick VRy      │
 GPIO 32 ◄──────┤ Joystick SW       │
                │                   │
 3.3V ──────────┤ Joystick VCC      │
 GND ───────────┤ Joystick GND      │
                └───────────────────┘
```

---

## 10. Wireless Connection

The transmitter ESP32 creates the Wi-Fi access point:

```text
SSID      : ESP_BUGGY
Password  : 12345678
```

The receiver ESP32 connects to this network.

The transmitter sends commands using UDP:

```text
UDP Port : 4210
```

The receiver listens for these packets and converts the received direction
into servo commands.

---

## 11. Command Flow

```text
Joystick Movement
       │
       ▼
Transmitter ESP32
       │
       ▼
Direction Detection
       │
       ▼
UDP Packet
       │
       ▼
Wi-Fi
       │
       ▼
Receiver ESP32
       │
       ├───────────────┐
       │               │
       ▼               ▼
Direction         HC-SR04
       │          Distance Check
       │               │
       └───────┬───────┘
               ▼
          Motor Control
               │
       ┌───────┴───────┐
       ▼               ▼
  Left Servo       Right Servo
```

---

## 12. Obstacle Protection

The receiver checks the ultrasonic sensor whenever a movement command is
received.

For a `FORWARD` command:

```text
Distance > 15 cm
       │
       ▼
Continue Forward
```

If:

```text
Distance ≤ 15 cm
       │
       ▼
Stop Both Servos
```

The current obstacle protection only blocks forward movement.

---

## 13. Servo Movement Logic

```text
FORWARD
Left Servo  → 0
Right Servo → 180

BACKWARD
Left Servo  → 180
Right Servo → 0

LEFT
Left Servo  → 0
Right Servo → 0

RIGHT
Left Servo  → 180
Right Servo → 180

STOP
Left Servo  → 90
Right Servo → 90
```

---

## 14. Safety Notes

- Do not power servo motors directly from ESP32 GPIO pins.
- Use an appropriate external supply for the servo motors.
- Connect all required grounds together.
- Do not connect a 5 V HC-SR04 ECHO signal directly to an ESP32 GPIO.
- Use a voltage divider or appropriate level shifter for the ECHO signal.
- Test the servo direction with the robot lifted from the surface before
  placing it on the ground.
- Keep the robot stationary during initial wiring and testing.

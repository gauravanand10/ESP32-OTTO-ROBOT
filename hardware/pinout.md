# Pin Configuration

This document contains the GPIO assignments used in the current
ESP32-OTTO-BUGGY implementation.

The system uses two ESP32 boards:

1. Transmitter ESP32
2. Receiver ESP32

---

## 1. Transmitter ESP32

The transmitter ESP32 reads the analog joystick and generates the movement
commands.

### GPIO Assignment

| Component | Pin | ESP32 GPIO | Function |
|---|---|---:|---|
| Joystick VRx | X-axis | GPIO 34 | Analog X-axis input |
| Joystick VRy | Y-axis | GPIO 35 | Analog Y-axis input |
| Joystick SW | Push button | GPIO 32 | Digital button input |

---

## Joystick Connections

```text
Analog Joystick          ESP32
────────────────────────────────
VRx                      GPIO 34
VRy                      GPIO 35
SW                       GPIO 32
VCC                      3.3V
GND                      GND
```

### Joystick Operation

The transmitter reads the X and Y analog values.

The calibrated center values currently used in the firmware are:

```text
Center X = 3010
Center Y = 2840
```

The dead zone is:

```text
Dead Zone = 200
```

This prevents small fluctuations around the joystick center from causing
unwanted robot movement.

---

## Direction Mapping

| Joystick Position | Direction |
|---|---|
| Up | FORWARD |
| Down | BACKWARD |
| Left | LEFT |
| Right | RIGHT |
| Center | STOP |

If both axes are active, the firmware compares their displacement and uses
the axis with the larger displacement to determine the direction.

---

## 2. Receiver ESP32

The receiver ESP32 controls the two continuous-rotation servo motors and
the HC-SR04 ultrasonic sensor.

### GPIO Assignment

| Component | ESP32 GPIO | Function |
|---|---:|---|
| Left Servo Signal | GPIO 13 | Left servo control |
| Right Servo Signal | GPIO 12 | Right servo control |
| HC-SR04 TRIG | GPIO 26 | Ultrasonic trigger |
| HC-SR04 ECHO | GPIO 25 | Ultrasonic echo |

---

## 3. Servo Pinout

### Left Servo

```text
Left Servo              ESP32
──────────────────────────────
Signal                   GPIO 13
VCC                      External 5V
GND                      Common GND
```

### Right Servo

```text
Right Servo             ESP32
──────────────────────────────
Signal                   GPIO 12
VCC                      External 5V
GND                      Common GND
```

The ESP32 GPIO provides the servo control signal only.

The servo motors should be powered from a suitable external supply.

---

## 4. HC-SR04 Pinout

```text
HC-SR04                  ESP32
──────────────────────────────
VCC                      5V
GND                      GND
TRIG                     GPIO 26
ECHO                     GPIO 25
```

### Important

The HC-SR04 is commonly powered from 5 V and its ECHO output can be 5 V.

ESP32 GPIOs are 3.3 V logic.

Therefore, when using a 5 V HC-SR04, the ECHO line should be connected to
GPIO 25 through an appropriate voltage divider or level-shifting circuit.

---

## 5. Complete GPIO Summary

### Transmitter

| GPIO | Connected Component |
|---:|---|
| GPIO 34 | Joystick VRx |
| GPIO 35 | Joystick VRy |
| GPIO 32 | Joystick SW |

### Receiver

| GPIO | Connected Component |
|---:|---|
| GPIO 13 | Left Servo Signal |
| GPIO 12 | Right Servo Signal |
| GPIO 26 | HC-SR04 TRIG |
| GPIO 25 | HC-SR04 ECHO |

---

## 6. Servo Command Mapping

The current firmware uses the following servo values:

| Command | Left Servo | Right Servo |
|---|---:|---:|
| FORWARD | 0 | 180 |
| BACKWARD | 180 | 0 |
| LEFT | 0 | 0 |
| RIGHT | 180 | 180 |
| STOP | 90 | 90 |

These values are specific to the current servo orientation on the robot.

---

## 7. Communication Configuration

The transmitter creates the Wi-Fi network:

```text
SSID      : ESP_BUGGY
Password  : 12345678
```

The receiver connects to the same network.

UDP communication uses:

```text
UDP Port : 4210
```

The receiver is configured with:

```text
Receiver IP : 192.168.4.2
```

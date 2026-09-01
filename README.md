# 🚗 ESP32-OTTO-ROBOT

> **A wireless ESP32 robotic robot controlled by an analog joystick, powered by UDP communication, and equipped with ultrasonic obstacle protection.**

<p align="center">

![ESP32](https://img.shields.io/badge/ESP32-Microcontroller-blue?style=for-the-badge&logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino-IDE-00979D?style=for-the-badge&logo=arduino)
![WiFi](https://img.shields.io/badge/Wi--Fi-UDP-orange?style=for-the-badge&logo=wifi)
![Platform](https://img.shields.io/badge/Platform-Otto%20Ninja-lightgrey?style=for-the-badge)

</p>

---

## 📌 Overview

**ESP32-OTTO-BUGGY** is a wireless robotic buggy built around **two ESP32
microcontrollers**.

An analog joystick is used as the primary controller. The transmitter ESP32
reads the joystick position, determines the required movement, and sends a
compact command over **Wi-Fi using UDP**.

The receiver ESP32 interprets the command and drives two
**continuous-rotation servo motors** using differential drive. An **HC-SR04
ultrasonic sensor** provides forward obstacle detection and prevents the
buggy from moving forward when an obstacle is too close.

The mechanical platform is based on a **3D-printed Otto Ninja design**.

---

## ✨ Features

- 🎮 Analog joystick control
- 📡 ESP32-to-ESP32 wireless communication
- ⚡ UDP-based command transmission
- 🚗 Forward and backward movement
- ↩️ Left and right turning
- 🛑 Stop control
- 📏 Ultrasonic obstacle detection
- 🖨️ 3D-printed Otto Ninja platform
- 🔧 Two-ESP32 transmitter/receiver architecture
- 💻 Arduino-compatible firmware

---

## 🧠 System Architecture

```text
                    🎮 ANALOG JOYSTICK
                           │
                     VRx / VRy / SW
                           │
                           ▼
                ┌─────────────────────┐
                │   ESP32 TRANSMITTER  │
                │                     │
                │  Read Joystick      │
                │  Direction Logic    │
                │  UDP Transmission   │
                └──────────┬──────────┘
                           │
                       Wi-Fi / UDP
                        Port 4210
                           │
                           ▼
                ┌─────────────────────┐
                │    ESP32 RECEIVER   │
                │                     │
                │  UDP Reception      │
                │  Motor Control      │
                │  Obstacle Check     │
                └───────┬───────┬─────┘
                        │       │
                        ▼       ▼
                   LEFT SERVO  RIGHT SERVO
                        │       │
                        └───┬───┘
                            ▼
                      🤖 OTTO BUGGY
                            ▲
                            │
                       HC-SR04
                  Obstacle Detection
```

---

## 🎮 Control Mapping

| Joystick Input | Buggy Action |
|---|---|
| ⬆️ Up | Forward |
| ⬇️ Down | Backward |
| ⬅️ Left | Turn Left |
| ➡️ Right | Turn Right |
| 🎯 Center | Stop |

The joystick uses calibrated X/Y center values and a dead zone to prevent
small analog fluctuations from generating unwanted movement commands.

---

## 📡 Wireless Communication

The transmitter ESP32 operates as a Wi-Fi access point.

```text
SSID      : ESP_BUGGY
Password  : 12345678
UDP Port  : 4210
```

The receiver ESP32 connects to this network and listens for incoming UDP
commands.

Example command:

```text
DIR:FORWARD,X:2500,Y:1200
```

The receiver extracts the `DIR` field and maps it to the corresponding motor
operation.

---

## ⚙️ Differential Drive

The two continuous-rotation servos are controlled independently.

| Movement | Left Servo | Right Servo |
|---|---:|---:|
| Forward | 0 | 180 |
| Backward | 180 | 0 |
| Left | 0 | 0 |
| Right | 180 | 180 |
| Stop | 90 | 90 |

These values correspond to the current physical orientation of the servos
in the prototype.

---

## 🚧 Ultrasonic Obstacle Protection

An **HC-SR04 ultrasonic sensor** is connected to the receiver ESP32.

The current forward obstacle threshold is:

```text
15 cm
```

When the buggy is commanded to move forward and an obstacle is detected
within this distance, the receiver stops both servos.

```text
             HC-SR04
                │
                ▼
        Measure Distance
                │
                ▼
        Distance ≤ 15 cm?
           /          \
         YES           NO
          │             │
          ▼             ▼
        STOP         FORWARD
```

---

## 🔌 Hardware

### Transmitter

The transmitter is built around an ESP32 and an analog joystick mounted on
a perfboard.

<img width="3024" height="4032" alt="transmitter" src="https://github.com/user-attachments/assets/96ed95cb-248e-404f-b077-b8aec9f7f609" />

### Buggy

The buggy uses an ESP32 receiver, two continuous-rotation servo motors,
and an HC-SR04 ultrasonic sensor mounted on the 3D-printed Otto Ninja
platform.

<img width="3024" height="4032" alt="buggy" src="https://github.com/user-attachments/assets/369a7c1c-11de-4c49-8bac-bc8ab309af64" />


## 📍 Pin Configuration

### Transmitter ESP32

| Component | GPIO | Function |
|---|---:|---|
| Joystick VRx | 34 | X-axis |
| Joystick VRy | 35 | Y-axis |
| Joystick SW | 32 | Push button |

### Receiver ESP32

| Component | GPIO | Function |
|---|---:|---|
| Left Servo | 13 | Left motor signal |
| Right Servo | 12 | Right motor signal |
| HC-SR04 TRIG | 26 | Trigger |
| HC-SR04 ECHO | 25 | Echo |

Detailed information is available in the [`hardware`](hardware/) directory.

---

## 💻 Firmware

The firmware is separated into transmitter and receiver programs.

### Transmitter

`firmware/transmitter/joystick_transmitter.ino`

Handles:

- Joystick acquisition
- Calibration
- Dead-zone processing
- Direction determination
- UDP packet generation
- Wireless transmission

### Receiver

`firmware/receiver/buggy_receiver.ino`

Handles:

- Wi-Fi connection
- UDP packet reception
- Direction extraction
- Servo motor control
- Ultrasonic distance measurement
- Forward obstacle protection

---

## 🖨️ 3D-Printed Platform

The buggy is based on the **Otto Ninja** mechanical platform.

The available STL files are organized under:

```text
3d-models/otto-ninja/
```

The folder contains the 3D-printable mechanical components used for the
platform.

---

## 📂 Repository Structure

```text
ESP32-OTTO-BUGGY/
│
├── README.md
│
├── firmware/
│   ├── transmitter/
│   │   └── joystick_transmitter.ino
│   │
│   └── receiver/
│       └── buggy_receiver.ino
│
├── hardware/
│   ├── components.md
│   ├── pinout.md
│   └── wiring.md
│
├── 3d-models/
│   └── otto-ninja/
│       └── *.stl
│
└── images/
    ├── transmitter.jpg
    └── buggy.jpg
```

---

## 🛠️ Setup

### 1. Upload the transmitter firmware

Open:

```text
firmware/transmitter/joystick_transmitter.ino
```

Upload it to the transmitter ESP32.

The ESP32 will create:

```text
ESP_BUGGY
```

### 2. Upload the receiver firmware

Open:

```text
firmware/receiver/buggy_receiver.ino
```

Upload it to the receiver ESP32.

The receiver will connect to the transmitter's Wi-Fi network and listen on
UDP port `4210`.

### 3. Connect the hardware

Follow the pin assignments and wiring documentation in:

```text
hardware/
```

### 4. Power the system

Power the transmitter and receiver and move the joystick to control the
buggy.

---

## ⚠️ Power & Wiring Notes

- Do **not** power servo motors directly from ESP32 GPIO pins.
- Use an appropriate external power supply for the servo motors.
- Maintain a common ground between the ESP32 and servo power supply.
- If the HC-SR04 is operated at 5 V, ensure that its ECHO signal is
  appropriately level-shifted before connecting it to an ESP32 GPIO.
- Verify servo orientation before running the buggy at full speed.

---

## 🔮 Future Development

The current version uses an **analog joystick** as the control interface.

Planned possibilities include:

- [ ] Improved obstacle avoidance
- [ ] Real-time telemetry
- [ ] Battery monitoring
- [ ] Additional sensors
- [ ] More advanced motion control
- [ ] Alternative human-machine interfaces
- [ ] Experimental EEG/EMG-based control

> **Note:** EEG/EMG control is future work and is not part of the current
> implementation.

---

## 📸 Project Status

**Current Status: Working Prototype ✅**

The current prototype successfully demonstrates:

```text
Joystick
   ↓
ESP32
   ↓
Wi-Fi / UDP
   ↓
ESP32
   ↓
Servo Motors
   ↓
Otto Ninja Buggy
```

---

<p align="center">
  Built with ESP32 • Arduino • Wi-Fi • UDP • Servo Control • Ultrasonic Sensing
</p>

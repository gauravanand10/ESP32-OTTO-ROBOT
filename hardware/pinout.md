# Pin Configuration

## Transmitter ESP32

The transmitter ESP32 reads the analog joystick and sends the
movement commands to the receiver through Wi-Fi.

| Component | ESP32 GPIO | Function |
|---|---:|---|
| Joystick VRx | GPIO 34 | X-axis |
| Joystick VRy | GPIO 35 | Y-axis |
| Joystick SW | GPIO 32 | Joystick button |

### Joystick Control

| Joystick Movement | Command |
|---|---|
| Up | FORWARD |
| Down | BACKWARD |
| Left | LEFT |
| Right | RIGHT |
| Center | STOP |

---

## Receiver ESP32

The receiver ESP32 controls the servo motors and ultrasonic sensor.

| Component | ESP32 GPIO | Function |
|---|---:|---|
| Left Servo | GPIO 13 | Left motor control |
| Right Servo | GPIO 12 | Right motor control |
| HC-SR04 TRIG | GPIO 26 | Trigger |
| HC-SR04 ECHO | GPIO 25 | Echo |

#include <WiFi.h>
#include <WiFiUdp.h>

// =========================
// Joystick Pins
// =========================

#define VRx 34
#define VRy 35
#define SW  32

// =========================
// Wi-Fi Configuration
// =========================

const char* ssid = "ESP_BUGGY";
const char* password = "12345678";

// Receiver ESP32 IP
const char* receiverIP = "192.168.4.2";

const int udpPort = 4210;

WiFiUDP udp;

// =========================
// Joystick Calibration
// =========================

const int centerX = 3010;
const int centerY = 2840;

const int deadZone = 200;

// =========================
// Setup
// =========================

void setup() {

  Serial.begin(115200);

  // Create Wi-Fi Access Point
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP32 BUGGY TRANSMITTER");
  Serial.println("================================");

  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());

  // Joystick pins
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);

  Serial.println("Joystick ready.");
}

// =========================
// Main Loop
// =========================

void loop() {

  // =========================
  // Read Joystick
  // =========================

  int xValue = analogRead(VRx);
  int yValue = analogRead(VRy);

  // Calculate difference from center
  int diffX = xValue - centerX;
  int diffY = yValue - centerY;

  // =========================
  // Determine Joystick State
  // =========================

  bool xActive = abs(diffX) > deadZone;
  bool yActive = abs(diffY) > deadZone;

  String direction = "STOP";

  // =========================
  // Direction Logic
  // =========================

  // X-axis only
  if (xActive && !yActive) {

    if (diffX > 0) {
      direction = "RIGHT";
    }
    else {
      direction = "LEFT";
    }
  }

  // Y-axis only
  else if (yActive && !xActive) {

    if (diffY > 0) {
      direction = "BACKWARD";
    }
    else {
      direction = "FORWARD";
    }
  }

  // Both axes active
  else if (xActive && yActive) {

    // Use the axis with greater displacement
    if (abs(diffX) > abs(diffY)) {

      if (diffX > 0) {
        direction = "RIGHT";
      }
      else {
        direction = "LEFT";
      }
    }

    else {

      if (diffY > 0) {
        direction = "BACKWARD";
      }
      else {
        direction = "FORWARD";
      }
    }
  }

  // =========================
  // Create UDP Message
  // =========================

  String message =
      "DIR:" + direction +
      ",X:" + String(xValue) +
      ",Y:" + String(yValue);

  // =========================
  // Send UDP Packet
  // =========================

  udp.beginPacket(receiverIP, udpPort);
  udp.print(message);
  udp.endPacket();

  // =========================
  // Serial Monitor
  // =========================

  Serial.print("Joystick X: ");
  Serial.print(xValue);

  Serial.print(" | Y: ");
  Serial.print(yValue);

  Serial.print(" | Direction: ");
  Serial.println(direction);

  delay(200);
}

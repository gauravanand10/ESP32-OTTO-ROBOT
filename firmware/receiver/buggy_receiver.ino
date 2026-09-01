#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>

// =========================
// Wi-Fi Configuration
// =========================

const char* ssid = "ESP_BUGGY";
const char* password = "12345678";

WiFiUDP udp;

const int udpPort = 4210;

// =========================
// Servo Configuration
// =========================

Servo leftServo;
Servo rightServo;

const int leftServoPin = 13;
const int rightServoPin = 12;

// Continuous rotation servos:
//
// 90  = STOP
// 0   = one direction
// 180 = opposite direction

// =========================
// Ultrasonic Sensor
// =========================

#define TRIG_PIN 26
#define ECHO_PIN 25

const int obstacleThreshold = 15;   // cm

// =========================
// Stop Buggy
// =========================

void stopBuggy() {

  leftServo.write(90);
  rightServo.write(90);
}

// =========================
// Get Distance
// =========================

long getDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(
    ECHO_PIN,
    HIGH,
    25000
  );

  // No echo
  if (duration == 0) {
    return 999;
  }

  long distance = duration * 0.034 / 2;

  return distance;
}

// =========================
// Setup
// =========================

void setup() {

  Serial.begin(115200);

  // Connect to transmitter Wi-Fi
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP32 BUGGY RECEIVER");
  Serial.println("================================");

  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println();

  Serial.println("Receiver connected!");

  Serial.print("Receiver IP: ");
  Serial.println(WiFi.localIP());

  // Start UDP
  udp.begin(udpPort);

  Serial.print("UDP Port: ");
  Serial.println(udpPort);

  // Attach servos
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  // Stop motors initially
  stopBuggy();

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Buggy ready.");
}

// =========================
// Main Loop
// =========================

void loop() {

  char incomingPacket[255];

  int packetSize = udp.parsePacket();

  if (packetSize) {

    int len = udp.read(
      incomingPacket,
      sizeof(incomingPacket) - 1
    );

    if (len > 0) {
      incomingPacket[len] = '\0';
    }

    String received = String(incomingPacket);

    Serial.print("Received: ");
    Serial.println(received);

    // =========================
    // Extract Direction
    // =========================

    String direction = "STOP";

    int separator = received.indexOf(',');

    if (separator > 4) {

      // Message format:
      // DIR:FORWARD,X:...,Y:...

      direction = received.substring(
        4,
        separator
      );
    }

    Serial.print("Direction: ");
    Serial.println(direction);

    // =========================
    // Read Ultrasonic
    // =========================

    long distance = getDistance();

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // =========================
    // Motor Control
    // =========================

    if (direction == "FORWARD") {

      // Obstacle protection
      if (distance <= obstacleThreshold) {

        stopBuggy();

        Serial.println(
          "OBSTACLE DETECTED -> STOP"
        );
      }

      else {

        // Move forward
        leftServo.write(0);
        rightServo.write(180);

        Serial.println("MOVING FORWARD");
      }
    }

    else if (direction == "BACKWARD") {

      // Move backward
      leftServo.write(180);
      rightServo.write(0);

      Serial.println("MOVING BACKWARD");
    }

    else if (direction == "LEFT") {

      // Rotate left
      leftServo.write(0);
      rightServo.write(0);

      Serial.println("TURNING LEFT");
    }

    else if (direction == "RIGHT") {

      // Rotate right
      leftServo.write(180);
      rightServo.write(180);

      Serial.println("TURNING RIGHT");
    }

    else {

      // STOP
      stopBuggy();

      Serial.println("STOP");
    }
  }
}

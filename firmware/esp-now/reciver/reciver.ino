#include <esp_now.h>
#include <WiFi.h>
#include "CytronMotorDriver.h"

// ===================== Spinner =====================
#define SPINNER 16

// ===================== Packet =====================
typedef struct {
  int x;
  int y;
  int swit;
  int order;
} Metal_Monster_Data;

Metal_Monster_Data receiverData;

// ===================== Motors =====================
// PWM, DIR

CytronMD motor1(
  PWM_DIR,
  12,
  22
);

CytronMD motor2(
  PWM_DIR,
  13,
  18
);

// ===================== Communication Failsafe =====================

volatile unsigned long lastPacketTime = 0;

const unsigned long CONNECTION_TIMEOUT = 500;

// ===================== Joystick Map =====================

inline int mapJoystickValue(
  int value,
  int inMin,
  int inMax,
  int outMin,
  int outMax
) {

  return map(
    value,
    inMin,
    inMax,
    outMin,
    outMax
  );
}

// ===================== Receive Callback =====================

void OnDataRecv(
  const esp_now_recv_info_t *info,
  const uint8_t *incomingData,
  int len
) {

  // Make sure packet size is correct
  if (len != sizeof(Metal_Monster_Data)) {

    Serial.print("Invalid packet size: ");
    Serial.println(len);

    return;
  }

  memcpy(
    &receiverData,
    incomingData,
    sizeof(receiverData)
  );

  // Packet received
  lastPacketTime = millis();

  int motor1Speed = 0;
  int motor2Speed = 0;

  const int maxSpeed = 255;

  // =====================
  // Debug
  // =====================

  Serial.print("RX -> ");

  Serial.print("X: ");
  Serial.print(receiverData.x);

  Serial.print(" | Y: ");
  Serial.print(receiverData.y);

  Serial.print(" | SW: ");
  Serial.print(receiverData.swit);

  Serial.print(" | ORDER: ");
  Serial.println(receiverData.order);

  // =====================
  // Motor Control
  // =====================

  switch (receiverData.order) {

    // ---------------------
    // STOP
    // ---------------------

    case 1:

      motor1Speed = 0;
      motor2Speed = 0;

      break;

    // ---------------------
    // RIGHT
    // ---------------------

    case 2:

      motor1Speed =
        mapJoystickValue(
          receiverData.y,
          2000,
          4095,
          0,
          maxSpeed
        );

      motor2Speed = 0;

      break;

    // ---------------------
    // LEFT
    // ---------------------

    case 3:

      motor1Speed = 0;

      motor2Speed =
        mapJoystickValue(
          receiverData.y,
          1500,
          0,
          0,
          maxSpeed
        );

      break;

    // ---------------------
    // BACKWARD
    // ---------------------

    case 4:

      motor1Speed =
        -mapJoystickValue(
          receiverData.x,
          2000,
          4095,
          0,
          maxSpeed
        );

      motor2Speed =
        -mapJoystickValue(
          receiverData.x,
          2000,
          4095,
          0,
          maxSpeed
        );

      break;

    // ---------------------
    // FORWARD
    // ---------------------

    case 5:

      motor1Speed =
        mapJoystickValue(
          receiverData.x,
          1800,
          0,
          0,
          maxSpeed
        );

      motor2Speed =
        mapJoystickValue(
          receiverData.x,
          1800,
          0,
          0,
          maxSpeed
        );

      break;

    // ---------------------
    // FORWARD LEFT
    // ---------------------

    case 6:
    {
      int speed =
        mapJoystickValue(
          receiverData.x,
          1800,
          0,
          0,
          maxSpeed
        );

      motor1Speed =
        speed * 0.7;

      motor2Speed =
        speed;

      break;
    }

    // ---------------------
    // FORWARD RIGHT
    // ---------------------

    case 7:
    {
      int speed =
        mapJoystickValue(
          receiverData.x,
          1800,
          0,
          0,
          maxSpeed
        );

      motor1Speed =
        speed;

      motor2Speed =
        speed * 0.7;

      break;
    }

    // ---------------------
    // BACKWARD LEFT
    // ---------------------

    case 8:
    {
      int speed =
        -mapJoystickValue(
          receiverData.y,
          1000,
          0,
          0,
          maxSpeed
        );

      motor1Speed =
        speed;

      motor2Speed =
        speed * 0.7;

      break;
    }

    // ---------------------
    // BACKWARD RIGHT
    // ---------------------

    case 9:
    {
      int speed =
        -mapJoystickValue(
          receiverData.y,
          1700,
          0,
          0,
          maxSpeed
        );

      motor1Speed =
        speed * 0.7;

      motor2Speed =
        speed;

      break;
    }

    // ---------------------
    // ROTATE LEFT
    // ---------------------

    case 10:

      motor1Speed = -maxSpeed;
      motor2Speed = maxSpeed;

      break;

    // ---------------------
    // ROTATE RIGHT
    // ---------------------

    case 11:

      motor1Speed = maxSpeed;
      motor2Speed = -maxSpeed;

      break;

    // ---------------------
    // UNKNOWN ORDER
    // ---------------------

    default:

      motor1Speed = 0;
      motor2Speed = 0;

      break;
  }

  // =====================
  // Limit Motor Values
  // =====================

  motor1Speed =
    constrain(
      motor1Speed,
      -255,
      255
    );

  motor2Speed =
    constrain(
      motor2Speed,
      -255,
      255
    );

  // =====================
  // Debug Motors
  // =====================

  Serial.print("M1 = ");
  Serial.print(motor1Speed);

  Serial.print(" | M2 = ");
  Serial.println(motor2Speed);

  // =====================
  // Apply Motor Speeds
  // =====================

  motor1.setSpeed(
    motor1Speed
  );

  motor2.setSpeed(
    motor2Speed
  );

  // =====================
  // Spinner
  // =====================

  digitalWrite(
    SPINNER,
    receiverData.swit
      ? LOW
      : HIGH
  );
}

// ===================== Setup =====================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("======================================");
  Serial.println("METAL MONSTER RECEIVER");
  Serial.println("======================================");

  // =====================
  // Motor safe state
  // =====================

  motor1.setSpeed(0);
  motor2.setSpeed(0);

  // =====================
  // Spinner
  // =====================

  pinMode(
    SPINNER,
    OUTPUT
  );

  digitalWrite(
    SPINNER,
    LOW
  );

  // =====================
  // WiFi
  // =====================

  WiFi.mode(WIFI_STA);

  delay(100);

  Serial.print(
    "Receiver MAC: "
  );

  Serial.println(
    WiFi.macAddress()
  );

  // =====================
  // ESP-NOW
  // =====================

  if (
    esp_now_init() != ESP_OK
  ) {

    Serial.println(
      "ESP-NOW INIT FAILED"
    );

    return;
  }

  // =====================
  // Receive Callback
  // =====================

  esp_now_register_recv_cb(
    OnDataRecv
  );

  lastPacketTime = millis();

  Serial.println(
    "Receiver Ready"
  );

  Serial.println(
    "Waiting for transmitter..."
  );
}

// ===================== Loop =====================

void loop() {

  // ==================================================
  // FAILSAFE
  // Stop motors if transmitter disappears
  // ==================================================

  if (
    millis() - lastPacketTime >
    CONNECTION_TIMEOUT
  ) {

    motor1.setSpeed(0);
    motor2.setSpeed(0);

    digitalWrite(
      SPINNER,
      LOW
    );
  }

  delay(20);
}

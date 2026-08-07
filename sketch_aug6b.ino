#include <esp_now.h>
#include <WiFi.h>
#include "CytronMotorDriver.h"

#define SPINNER 16

typedef struct {
  int x;
  int y;
  int swit;
  int order;
} Metal_Monster_Data;

Metal_Monster_Data receiverData;

// Correct pin order: PWM, DIR
CytronMD motor1(PWM_DIR, 12, 22);
CytronMD motor2(PWM_DIR, 13, 18);

inline int mapJoystickValue(int value, int inMin, int inMax, int outMin, int outMax) {
  return map(value, inMin, inMax, outMin, outMax);
}

// ESP32 Arduino Core 3.x callback
void OnDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *incomingData,
                int len)
{
  memcpy(&receiverData, incomingData, sizeof(receiverData));

  int motor1Speed = 0;
  int motor2Speed = 0;
  const int maxSpeed = 255;

  Serial.print("Order: ");
  Serial.println(receiverData.order);

  switch (receiverData.order)
  {
    case 1: // Stop
      motor1Speed = 0;
      motor2Speed = 0;
      break;

    case 2: // Right
      motor1Speed = mapJoystickValue(receiverData.y, 2000, 4095, 0, maxSpeed);
      motor2Speed = 0;
      break;

    case 3: // Left
      motor1Speed = 0;
      motor2Speed = mapJoystickValue(receiverData.y, 1500, 0, 0, maxSpeed);
      break;

    case 4: // Backward
      motor1Speed = -mapJoystickValue(receiverData.x, 2000, 4095, 0, maxSpeed);
      motor2Speed = -mapJoystickValue(receiverData.x, 2000, 4095, 0, maxSpeed);
      break;

    case 5: // Forward
      motor1Speed = mapJoystickValue(receiverData.x, 1800, 0, 0, maxSpeed);
      motor2Speed = mapJoystickValue(receiverData.x, 1800, 0, 0, maxSpeed);
      break;

    case 6: // Forward Left
    {
      int speed = mapJoystickValue(receiverData.x, 1800, 0, 0, maxSpeed);
      motor1Speed = speed * 0.7;
      motor2Speed = speed;
      break;
    }

    case 7: // Forward Right
    {
      int speed = mapJoystickValue(receiverData.x, 1800, 0, 0, maxSpeed);
      motor1Speed = speed;
      motor2Speed = speed * 0.7;
      break;
    }

    case 8: // Backward Left
    {
      int speed = -mapJoystickValue(receiverData.y, 1000, 0, 0, maxSpeed);
      motor1Speed = speed;
      motor2Speed = speed * 0.7;
      break;
    }

    case 9: // Backward Right
    {
      int speed = -mapJoystickValue(receiverData.y, 1700, 0, 0, maxSpeed);
      motor1Speed = speed * 0.7;
      motor2Speed = speed;
      break;
    }

    case 10: // Rotate Left
      motor1Speed = -maxSpeed;
      motor2Speed = maxSpeed;
      break;

    case 11: // Rotate Right
      motor1Speed = maxSpeed;
      motor2Speed = -maxSpeed;
      break;

    default:
      motor1Speed = 0;
      motor2Speed = 0;
      break;
  }

  Serial.print("M1 = ");
  Serial.print(motor1Speed);
  Serial.print("   M2 = ");
  Serial.println(motor2Speed);

  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);

  digitalWrite(SPINNER, receiverData.swit ? LOW : HIGH);
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  pinMode(SPINNER, OUTPUT);
  digitalWrite(SPINNER, LOW);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver Ready");
}

void loop()
{
}
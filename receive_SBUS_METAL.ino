#include "sbus.h"
#include "CytronMotorDriver.h"

#define SBUS_RX_PIN 21
#define SBUS_TX_PIN -1


// CHANGE THIS PIN (can't be 16 because SBUS uses it)
#define SPINNER 16

bfs::SbusRx sbus_rx(&Serial2, SBUS_RX_PIN, SBUS_TX_PIN, true);
bfs::SbusData data;

CytronMD motor1(PWM_DIR, 22, 12);
CytronMD motor2(PWM_DIR, 18, 13);

const int MAX_SPEED = 255;

// ---------------------------------------------------

int mapSBUS(int value, int outMin, int outMax)
{
  value = constrain(value, 172, 1811);
  return map(value, 172, 1811, outMin, outMax);
}

// ---------------------------------------------------

void setup()
{
  Serial.begin(115200);

  pinMode(SPINNER, OUTPUT);
  digitalWrite(SPINNER, HIGH);

  sbus_rx.Begin();

  Serial.println("SBUS Robot Ready");
}

// ---------------------------------------------------

void loop()
{
  if (!sbus_rx.Read())
    return;

  data = sbus_rx.data();

  //-----------------------------
  // Channels
  //-----------------------------

  int ch1 = data.ch[0];   // Steering
  int ch2 = data.ch[1];   // Throttle
  int ch5 = data.ch[4];   // Spinner switch

  //-----------------------------
  // Convert SBUS -> Motor values
  //-----------------------------

  int throttle = mapSBUS(ch2, -255, 255);
  int steering = mapSBUS(ch1, -255, 255);

  // Small deadband
  if (abs(throttle) < 15)
    throttle = 0;

  if (abs(steering) < 15)
    steering = 0;

  //-----------------------------
  // Differential Drive Mixing
  //-----------------------------

  int motor1Speed = throttle + steering;
  int motor2Speed = throttle - steering;

  motor1Speed = constrain(motor1Speed, -255, 255);
  motor2Speed = constrain(motor2Speed, -255, 255);

  //-----------------------------
  // Drive Motors
  //-----------------------------

  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);

  //-----------------------------
  // Spinner
  //-----------------------------

  bool spinner = (ch5 > 1000);

  digitalWrite(SPINNER, spinner ? LOW : HIGH);

  //-----------------------------
  // Debug
  //-----------------------------

  Serial.print("CH1: ");
  Serial.print(ch1);

  Serial.print("  CH2: ");
  Serial.print(ch2);

  Serial.print("  CH5: ");
  Serial.print(ch5);

  Serial.print("  M1: ");
  Serial.print(motor1Speed);

  Serial.print("  M2: ");
  Serial.print(motor2Speed);

  Serial.print("  Spinner: ");
  Serial.println(spinner);

  delay(20);
}
#include <esp_now.h>
#include <WiFi.h>

// ===================== Pin Definitions =====================
#define PinJoys_y 34
#define PinJoys_x 35

#define sw 12
#define switch_sys 14
#define switch_rotate_left 15
#define switch_rotate_right 13

// ===================== Direction Orders =====================
#define stop 1
#define right 2
#define left 3
#define backward 4
#define forward 5
#define forward_left 6
#define forward_right 7
#define backward_left 8
#define backward_right 9
#define rotate_left 10
#define rotate_right 11

// ===================== Globals =====================
volatile bool canSend = true;
volatile bool receiverConfirmed = false;

int system_switch = 0;
int x = 0;
int y = 0;

// ==========================================================
// CORRECT RECEIVER MAC
// ==========================================================


// ===================== Data Packet =====================
typedef struct Metal_Monster_Data {
  int x;
  int y;
  int swit;
  int order;
} Metal_Monster_Data;

Metal_Monster_Data Data;

// ===================== Send Callback =====================
// Arduino ESP32 Core 3.x
void OnDataSent(
  const wifi_tx_info_t *tx_info,
  esp_now_send_status_t status
) {
  (void)tx_info;

  canSend = true;

  if (status == ESP_NOW_SEND_SUCCESS) {

    if (!receiverConfirmed) {

      receiverConfirmed = true;

      Serial.println();
      Serial.println("======================================");
      Serial.println("ESP-NOW COMMUNICATION SUCCESSFUL");
      Serial.println("Receiver: 80:F3:DA:42:64:00");
      Serial.println("Packet delivered successfully");
      Serial.println("======================================");
      Serial.println();
    }

  } else {

    if (!receiverConfirmed) {
      Serial.println("Waiting for receiver...");
    }
  }
}

// ===================== Send Task =====================
void sendDataTask(void *param) {

  (void)param;

  while (true) {

    if (canSend) {

      esp_err_t result = esp_now_send(
        receiverAddress,
        (uint8_t *)&Data,
        sizeof(Data)
      );

      if (result == ESP_OK) {

        canSend = false;

      } else {

        Serial.print("esp_now_send error: ");
        Serial.println(result);
      }
    }

    // About 100 packets/sec maximum
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// ===================== Input Task =====================
void readInputTask(void *param) {

  (void)param;

  while (true) {

    // =====================
    // Read Joystick
    // =====================

    y = analogRead(PinJoys_y);
    x = analogRead(PinJoys_x);

    int k = digitalRead(sw);

    int rotation_left_read =
      digitalRead(switch_rotate_left);

    int rotation_right_read =
      digitalRead(switch_rotate_right);

    system_switch =
      digitalRead(switch_sys);

    // =====================
    // Fill packet
    // =====================

    Data.x = x;
    Data.y = y;
    Data.swit = k;

    // ======================================================
    // ROTATION BUTTONS
    // checked first
    // INPUT_PULLUP -> pressed = LOW
    // ======================================================

    if (rotation_right_read == LOW) {

      Data.order = rotate_right;
    }

    else if (rotation_left_read == LOW) {

      Data.order = rotate_left;
    }

    // =====================
    // STOP
    // =====================

    else if (
      y > 1700 &&
      y < 2100 &&
      x > 1900 &&
      x < 2100
    ) {

      Data.order = stop;
    }

    // =====================
    // RIGHT
    // =====================

    else if (
      x < 2500 &&
      x > 1000 &&
      y > 2000
    ) {

      Data.order = right;
    }

    // =====================
    // LEFT
    // =====================

    else if (
      x < 2500 &&
      x > 1000 &&
      y < 1500
    ) {

      Data.order = left;
    }

    // =====================
    // BACKWARD
    // =====================

    else if (
      y < 2500 &&
      y > 1000 &&
      x > 2000
    ) {

      Data.order = backward;
    }

    // =====================
    // FORWARD
    // =====================

    else if (
      y < 2500 &&
      y > 1000 &&
      x < 1800
    ) {

      Data.order = forward;
    }

    // =====================
    // FORWARD LEFT
    // =====================

    else if (
      x > 2500 &&
      y > 2500
    ) {

      Data.order = forward_left;
    }

    // =====================
    // FORWARD RIGHT
    // =====================

    else if (
      x < 1000 &&
      y > 2500
    ) {

      Data.order = forward_right;
    }

    // =====================
    // BACKWARD LEFT
    // =====================

    else if (
      y < 1000 &&
      x > 2500
    ) {

      Data.order = backward_left;
    }

    // =====================
    // BACKWARD RIGHT
    // =====================

    else if (
      y < 1000 &&
      x < 1000
    ) {

      Data.order = backward_right;
    }

    // =====================
    // DEFAULT STOP
    // =====================

    else {

      Data.order = stop;
    }

    // =====================
    // Serial Debug
    // =====================

    Serial.print("X: ");
    Serial.print(x);

    Serial.print(" | Y: ");
    Serial.print(y);

    Serial.print(" | SW: ");
    Serial.print(k);

    Serial.print(" | Rotate L: ");
    Serial.print(rotation_left_read);

    Serial.print(" | Rotate R: ");
    Serial.print(rotation_right_read);

    Serial.print(" | ORDER: ");
    Serial.print(Data.order);

    Serial.print(" | LINK: ");

    if (receiverConfirmed) {
      Serial.println("OK");
    } else {
      Serial.println("WAITING");
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// ===================== Setup =====================
void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("======================================");
  Serial.println("METAL MONSTER TRANSMITTER");
  Serial.println("======================================");

  // =====================
  // Initial safe packet
  // =====================

  Data.x = 0;
  Data.y = 0;
  Data.swit = HIGH;
  Data.order = stop;

  // =====================
  // Pins
  // =====================

  pinMode(PinJoys_y, INPUT);
  pinMode(PinJoys_x, INPUT);

  pinMode(sw, INPUT_PULLUP);
  pinMode(switch_sys, INPUT_PULLUP);

  pinMode(
    switch_rotate_left,
    INPUT_PULLUP
  );

  pinMode(
    switch_rotate_right,
    INPUT_PULLUP
  );

  // =====================
  // WiFi
  // =====================

  WiFi.mode(WIFI_STA);

  delay(100);

  Serial.print("Transmitter MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.println(
    "Target Receiver: 80:F3:DA:42:64:00"
  );

  // =====================
  // ESP-NOW Init
  // =====================

  if (esp_now_init() != ESP_OK) {

    Serial.println("ESP-NOW INIT FAILED");

    return;
  }

  Serial.println("ESP-NOW initialized");

  // =====================
  // Register Send Callback
  // =====================

  esp_now_register_send_cb(OnDataSent);

  // =====================
  // Add Receiver Peer
  // =====================

  esp_now_peer_info_t peerInfo = {};

  memcpy(
    peerInfo.peer_addr,
    receiverAddress,
    6
  );

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_err_t peerResult =
    esp_now_add_peer(&peerInfo);

  if (peerResult != ESP_OK) {

    Serial.print(
      "Failed to add receiver. Error: "
    );

    Serial.println(peerResult);

    return;
  }

  // =====================
  // Verify Peer
  // =====================

  if (
    esp_now_is_peer_exist(receiverAddress)
  ) {

    Serial.println(
      "Receiver peer registered successfully"
    );

  } else {

    Serial.println(
      "Receiver peer registration FAILED"
    );

    return;
  }

  Serial.println();
  Serial.println("Waiting for receiver...");
  Serial.println();

  // =====================
  // FreeRTOS Tasks
  // =====================

  xTaskCreatePinnedToCore(
    sendDataTask,
    "Send Data",
    4096,
    NULL,
    2,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    readInputTask,
    "Read Inputs",
    4096,
    NULL,
    3,
    NULL,
    1
  );
}

// ===================== Loop =====================
void loop() {

  delay(1000);
}

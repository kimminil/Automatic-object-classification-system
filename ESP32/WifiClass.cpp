#include "WifiClass.h"
#include <Arduino.h>
QueueHandle_t que = NULL;

WifiClass::WifiClass(char *ssid, char *password)
  : server(80), color(""), lastColor(""), servoPin(12), servoPin2(13) {
  this->ssid = ssid;
  this->password = password;
  this->servo.attach(this->servoPin);
  this->servo2.attach(this->servoPin2);
  this->servo.write(180);
  this->servo2.write(90);
  que = xQueueCreate(10, sizeof(int));
}

void WifiClass::servoResponse(void *pvParameters) {
  WifiClass *wf = (WifiClass *)pvParameters;

  while (1) {
    WiFiClient client = wf->server.available();
    vTaskDelay(1);
    if (client) {  // if you get a client,

      Serial.println("New Client.");  // print a message out the serial port

      String currentLine = "";  // make a String to hold incoming data from the client

      while (client.connected()) {  // loop while the client's connected

        if (client.available()) {  // if there's bytes to read from the client,

          wf->color = client.readStringUntil('\n');  // read a byte, then
          Serial.println(wf->color);
          int cmd;
          if (wf->lastColor != wf->color) {
            if (wf->color == "R") {
              cmd = 1;
              //Serial.println("Red");
              xQueueSend(que, &cmd, 0);
              wf->lastColor = wf->color;
            } else if (wf->color == "B") {

              cmd = 2;
              //Serial.println("Blue");
              xQueueSend(que, &cmd, 0);
              wf->lastColor = wf->color;
            }
            if (wf->color == "C") {
              //Serial.println("ETC");
              wf->servo.write(0);
              vTaskDelay(1000);
              wf->servo.write(180);
            }
          }
          //Serial.println(c);
        }
      }
      // close the connection:
      client.stop();

      Serial.println("Client Disconnected.");
    }
    vTaskDelay(10);
  }
}
void WifiClass::servoOp(void *pvParameters) {
  WifiClass *wf = (WifiClass *)pvParameters;
  int result;
  while (1) {
    if (xQueueReceive(que, &result, 0)) {     // 큐에 값이 올 때까지 대기
      vTaskDelay(1000 / portTICK_PERIOD_MS);  // 5초 대기
      if (result == 1) {
        wf->servo2.write(120);
      } else if (result == 2) {
        wf->servo2.write(60);
      }
    }
  }
}

void WifiClass::wifiOn() {
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");

  Serial.println(myIP);
  server.begin();
}
void WifiClass::servoInit() {
}

void WifiClass::onTask() {
  xTaskCreate(
    servoResponse,         // 태스크 함수
    "Servo Control Task",  // 태스크 이름
    2048,                  // 스택 크기
    this,                  // this 포인터 전달
    2,                     // 우선순위
    NULL                   // 태스크 핸들 (필요시)
  );

  xTaskCreate(
    servoOp,                 // 태스크 함수
    "Servo Operation Task",  // 태스크 이름
    2048,                    // 스택 크기
    this,                    // this 포인터 전달
    1,                       // 우선순위
    NULL                     // 태스크 핸들 (필요시)
  );
}

#ifndef WIFICLASST_H
#define WIFICLASST_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ESP32Servo.h>
extern QueueHandle_t que;
class WifiClass {
private:
  char *ssid;
  char *password;
  WiFiServer server;
  String color;
  String lastColor;
  uint16_t servoPin;
  uint16_t servoPin2;
  Servo servo;
  Servo servo2;
public:
  WifiClass(char *ssid, char *password);
  void wifiOn();
  static void servoResponse(void *pvParameters) ;
  void servoInit();
  // static void SV_CONTROL(void *pvParameters);    // static으로 선언
  void onTask();
  static void servoOp(void* pvParameters);
};

#endif

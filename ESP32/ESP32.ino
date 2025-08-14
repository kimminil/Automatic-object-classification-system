#include <ESP32Servo.h>
#include "stepping_motor.h"
stepping_m st1(13,12,14,27);
servo servo1;
void setup() {
servo.attach(12);
servo.attach(13);
servo.write(180);
  
}

void loop() {
  st1.st_act();
}

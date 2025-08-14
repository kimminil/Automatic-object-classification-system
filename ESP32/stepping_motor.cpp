#include "stepping_motor.h"

stepping_m::stepping_m(int st_in1, int st_in2, int st_in3, int st_in4) {
  this->st_in1 = st_in1;
  this->st_in2 = st_in2;
  this->st_in3 = st_in3;
  this->st_in4 = st_in4;
  pinMode(this->st_in1, OUTPUT);
  pinMode(this->st_in2, OUTPUT);
  pinMode(this->st_in3, OUTPUT);
  pinMode(this->st_in4, OUTPUT);
}
// 아날로그 신호를 받고 반환
void stepping_m::st_act() {
  digitalWrite(this->st_in1, LOW);
  digitalWrite(this->st_in2, HIGH);//1
  digitalWrite(this->st_in3, HIGH);// 1
  digitalWrite(this->st_in4, LOW);
  delay(5);

  digitalWrite(this->st_in1, LOW);
  digitalWrite(this->st_in2, HIGH); //1
  digitalWrite(this->st_in3, LOW);
  digitalWrite(this->st_in4, HIGH); //1
  delay(5);

  digitalWrite(this->st_in1, HIGH);
  digitalWrite(this->st_in2, LOW);
  digitalWrite(this->st_in3, LOW);
  digitalWrite(this->st_in4, HIGH);
  delay(5);

  digitalWrite(this->st_in1, HIGH);
  digitalWrite(this->st_in2, LOW);
  digitalWrite(this->st_in3, HIGH);
  digitalWrite(this->st_in4, LOW);
  delay(5);
}

#ifndef stepping_M
#define stepping_M
#include <Arduino.h>
class stepping_m{
private:
  int st_in1;
  int st_in2;
  int st_in3;
  int st_in4;
public:
  stepping_m(int st_in1, int st_in2, int st_in3, int st_in4);
  void st_act();
};

#endif
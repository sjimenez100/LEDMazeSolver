#include <ArduinoSTL.h>
#include <array>
#include "TT.h"

void setup(){
  Serial.begin(9600);
  std::array<uint8_t, 2> a = {3, 3};

  TAX tt(a);


  Serial.println(tt.a[0]);
}
void loop(){}
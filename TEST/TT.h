#ifndef TT_H
#define TT_H

#include <ArduinoSTL.h>
#include <array>

class TAX {

public:
  std::array<uint8_t, 2> a;
  
  TAX(std::array<uint8_t, 2> x) : a(x){}
};


#endif
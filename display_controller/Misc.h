#ifndef MISC_H
#define MISC_H

#include <ArduinoSTL.h>
#include <array>

using namespace std;

static std::array<int, 2> add(std::array<int, 2> v1, std::array<int, 2> v2) {
  array<int, 2> vn = { v1[0] + v2[0], v1[1] + v2[1] };
  return vn;
}

static void assert(bool condition, const char *message){
  if(!condition){
    Serial.print(message);
    while(1);
  }
}

// modified from https://www.programiz.com/cpp-programming/library-function/cstdlib/qsort
static int compare(const void* a, const void* b) {
  const uint8_t* x = (uint8_t*)a;
  const uint8_t* y = (uint8_t*)b;

  if (*x > *y)
    return 1;
  else if (*x < *y)
    return -1;

  return 0;
}

template<typename T, typename F>
static void arrayMap(T rawArray[], int length, F func){
  for(int i = 0; i < length; i++){
    rawArray[i] = func(i, rawArray);
  }
}


#endif
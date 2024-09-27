#ifndef MISC_H
#define MISC_H

#include <array>

using namespace std;

static std::array<int, 2> add(std::array<int, 2> v1, std::array<int, 2> v2) {
  array<int, 2> vn = { v1[0] + v2[0], v1[1] + v2[1] };
  return vn;
}

template<typename T, typename F>
static void arrayMap(T rawArray[], int length, F func) {
  for (int i = 0; i < length; i++) {
    rawArray[i] = func(i, rawArray);
  }
}


#endif
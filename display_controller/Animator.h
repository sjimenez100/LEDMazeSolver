#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "CoRoutine.h"
#include "Display.h"
#include <ArduinoSTL.h>
#include <array>

using namespace std;

template<typename T>
class Animator {

private:
  int index = 0;
  T* base; 
  Display<uint8_t> display;
  std::vector<array<uint8_t, 2>> positions;
  unsigned long duration;
  int length;
  int columns;
  unsigned long period;
  CoRoutine routine;
  bool loop;

public:
  Animator(Display<uint8_t> display, unsigned long duration, bool loop=false) : 
   display(display), duration(duration), loop(loop) {
    
  }

  void initialize(T base[], const vector<array<uint8_t, 2>>& positions){
    this->base = base;
    this->positions = positions;
    length = positions.size();
    columns = sizeof(T) * 8;
    period = duration/length;
    routine.period = period;
  }

  bool animate(){

    auto func = [=](){

      if(index){
        array<uint8_t, 2>& previousPosition = positions[index-1];
        uint8_t rp = previousPosition[0];
        uint8_t cp = previousPosition[1];

        // set value of previous off
        base[rp] = base[rp] & ~(1 << (columns - 1) - cp);
      }

      array<uint8_t, 2>& currentPosition = positions[index];
      uint8_t rc = currentPosition[0];
      uint8_t cc = currentPosition[1];

      // set value of current on
      base[rc] = base[rc] | 1 << (columns - 1) - cc;
      
      display.refresh(base);

      if(index == length-1)
      {
        // set value of current off
        base[rc] = base[rc] & ~(1 << (columns - 1) - cc);

        index = 0;
        return !loop;
      }

      index++;
      return false;
    };

    return routine.execute(func);
  }

};

#endif
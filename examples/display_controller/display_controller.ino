#include <ArduinoSTL.h>


"LEDMazeSolver\examples\display_controller\display_controller.ino"

"LEDMazeSolver\src\Display.h"


#include "..\display_controller\Display.h"


#include "Maze.h"

#include <..\src\iostream>
#include <..\src\bitset>

using namespace std;

const uint8_t dataPin = 11;
const uint8_t clockPin = 10;
const uint8_t latchPin = 9;

const uint8_t togglePin = A5;


Display display(dataPin, clockPin, latchPin);
Maze<uint8_t> maze(8);

bool toggle = false;


void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(togglePin, INPUT);


  Serial.begin(9600);
  Serial.println();

  // pin 0 must remain unconnected
  uint8_t seed = analogRead(0);
  randomSeed(seed);


  maze.generate();
  maze.printGrid();
}

void loop() {

  if (digitalRead(togglePin)) {
    if (!toggle) {
      toggle = true;
      maze.generate();
      //maze.printGrid();
    }
  } else {
    toggle = false;
  }

  display.refresh(70.0, maze.grid);
}

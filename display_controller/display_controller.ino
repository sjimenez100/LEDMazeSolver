#include "Display.h"
#include "Animator.h"
#include "Maze.h"
#include "Test.h"
#include "PathFinding.h"

#include <ArduinoSTL.h>
#include <array>
#include <vector>

using namespace std;

const uint8_t dataPin = 12;
const uint8_t latchPin = 11;
const uint8_t clockPin = 10;

const uint8_t togglePin = 2;
bool toggle = false;

Display<uint8_t> display(dataPin, clockPin, latchPin, 8);
Maze<uint8_t> maze(8);
Animator<uint8_t> animator(display, 2000, false);


const array<uint8_t, 2> startPosition = {0, 0};
const array<uint8_t, 2> targetPosition = {6, 6};

void setup() {
  Serial.begin(9600);
  
  pinMode(togglePin, INPUT);

  uint8_t seed = analogRead(0);
  randomSeed(seed);

  display.initialize();
  maze.generate();
  display.refresh(maze.grid);


  const vector<array<uint8_t, 2>>& path = AStar<uint8_t, 8, 8>(maze.grid, startPosition, targetPosition);
  animator.initialize(maze.grid, path);
}


void loop() {

  bool animationCompleted = animator.animate();

  if(animationCompleted){
    maze.generate();
    const vector<array<uint8_t, 2>>& path = AStar<uint8_t, 8, 8>(maze.grid, startPosition, targetPosition);
    animator.initialize(maze.grid, path);
  }
  
}

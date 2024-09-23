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

Display<uint8_t> display(dataPin, clockPin, latchPin, 16);
Maze<uint8_t> maze(16);
Animator<uint8_t> animator(display, 2000, true);

bool a = true;
void setup() {

  Serial.begin(9600);
  delay(100);
  Serial.println();
  
  pinMode(togglePin, INPUT);

  uint8_t seed = analogRead(0);
  randomSeed(seed);

  maze.generate();
  display.initialize();

  array<int, 2> startPosition = {0, 0};
  array<int, 2> targetPosition = {14, 6};

  Serial.println("TEST1");
  vector<array<int, 2>> path = AStar<uint8_t, 16, 8>(maze.grid, startPosition, targetPosition);
  Serial.println(path.size());
  animator.initialize(maze.grid, path);

  display.refresh(maze.grid);
  maze.printGrid();
}


void loop() {
    animator.animate();
}

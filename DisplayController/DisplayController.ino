#include <array>
#include <vector>

#include "MAX7219.h"
#include "Maze.h"
#include "Animator.h"
#include "PathFinding.h"

using namespace std;

const uint8_t dataPin = 7;
const uint8_t latchPin = 6;
const uint8_t clockPin = 5;

const std::array<uint8_t, 2> startPosition = { 0, 0 };
const std::array<uint8_t, 2> targetPosition = { 14, 15 };

MAX7219<uint16_t> display(dataPin, clockPin, latchPin, 16);
Maze<uint16_t> maze(16);
Animator<uint16_t> animator(display, 3250, false);

void setup() {
  Serial.begin(9600);

  uint8_t seed = analogRead(1);
  randomSeed(seed);

  maze.generate();
  display.initialize();
  display.refresh(maze.grid);
  const vector<array<uint8_t, 2>>& path = AStar<uint16_t, 16, 16>(maze.grid, startPosition, targetPosition);
  animator.initialize(maze.grid, path);
}


void loop() {
  bool animationCompleted = animator.animate();

  if (animationCompleted) {
    maze.generate();
    const vector<array<uint8_t, 2>>& path = AStar<uint16_t, 16, 16>(maze.grid, startPosition, targetPosition);
    animator.initialize(maze.grid, path);
  }
}

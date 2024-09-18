#include "Display.h"
#include "Animator.h"
#include "Maze.h"
#include "Test.h"

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
vector<array<int, 2>> positions;
Animator<uint8_t> animator(display, 1000, true);

bool a = true;
void setup() {

  pinMode(togglePin, INPUT);

  Serial.begin(9600);
  Serial.println();

  uint8_t seed = analogRead(0);
  randomSeed(seed);

  maze.generate();
  display.initialize();
  animator.initialize(maze.grid, positions);
  for(int i=0; i<8; i++){
    Serial.println(maze.grid[i]);
  }

  positions.push_back({0, 0});
  positions.push_back({1, 1});
  positions.push_back({2, 2});
  positions.push_back({3, 3});
  positions.push_back({4, 3});
  positions.push_back({5, 2});
  positions.push_back({6, 1});
  positions.push_back({7, 0});

  
}

void loop() {

  
}

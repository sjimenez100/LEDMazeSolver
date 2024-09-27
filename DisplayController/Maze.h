#ifndef MAZE_H
#define MAZE_H

//#include <StandardCplusplus.h>
#include <stack>
#include "Misc.h"

using namespace std;

template<typename T>
class Maze {

private:

  bool inverted = false;

  // returns 1, 2, 3, 4 correspining to N, E, S, W or 0 if there are no unvisited neighbours
  uint8_t checkNeighbours(std::array<int, 2> currentCell, T visited[]) {
    uint8_t row = currentCell[0];
    uint8_t col = currentCell[1];

    vector<int> unvisitedStates;

    if (row >= 2) {
      if (!(visited[row - 2] & (1 << (columns - 1) - col))) {
        unvisitedStates.push_back(1);
      }
    }

    if (col <= columns - 3) {
      if (!(visited[row] & (1 << (columns - 1) - col - 2))) {
        unvisitedStates.push_back(2);
      }
    }
    if (row <= rows - 3) {
      if (!(visited[row + 2] & (1 << (columns - 1) - col))) {
        unvisitedStates.push_back(3);
      }
    }

    if (col >= 2) {
      if (!(visited[row] & (1 << (columns - 1) - col + 2))) {
        unvisitedStates.push_back(4);
      }
    }

    if (!unvisitedStates.empty()) {
      return unvisitedStates[random(0, unvisitedStates.size())];
    }

    return 0;
  }

  void DFS(T grid[], std::array<int, 2> initialCell) {

    vector<array<int, 2>> neighbourOffsets;
    neighbourOffsets.push_back({ -2, 0 });
    neighbourOffsets.push_back({ 0, 2 });
    neighbourOffsets.push_back({ 2, 0 });
    neighbourOffsets.push_back({ 0, -2 });

    T visited[rows]{};
    std::stack<std::array<int, 2>> cellStack;

    visited[initialCell[0]] = 1 << (columns - 1) - initialCell[1];
    cellStack.push(initialCell);

    while (!cellStack.empty()) {
      // Serial.println("-----------------");
      // Serial.println("-----------------");

      std::array<int, 2> currentCell = cellStack.top();
      cellStack.pop();

      //printVec(currentCell, "Current Cell");

      uint8_t neighbourState = checkNeighbours(currentCell, visited);

      //Serial.print("NeighbourState: ");
      //Serial.println(neighbourState);

      if (neighbourState) {
        cellStack.push(currentCell);

        std::array<int, 2> offset = neighbourOffsets[neighbourState - 1];
        //printVec(offset, "offset");

        std::array<int, 2> neighbour = add(currentCell, offset);
        //printVec(neighbour, "neighbour");

        std::array<int, 2> woffset = { (int)(offset[0] / 2), (int)(offset[1] / 2) };
        //printVec(woffset, "woffset");

        std::array<int, 2> wall = add(currentCell, woffset);
        //printVec(wall, "wall");

        grid[wall[0]] = grid[wall[0]] | 1 << (columns - 1) - wall[1];

        visited[neighbour[0]] = visited[neighbour[0]] | 1 << (columns - 1) - neighbour[1];

        cellStack.push(neighbour);
      }
    }
  }

  std::array<int, 2> randomInitialCell() {

    uint8_t evenRowIndex[rows / 2]{};
    uint8_t evenColumnIndex[columns / 2]{};

    // int oddRowIndex[rows/2]{};
    // int oddColumnIndex[columns/2]{};

    for (int i = 0; i < rows / 2; i++) {
      evenRowIndex[i] = 2 * i;
    }

    for (int i = 0; i < columns / 2; i++) {
      evenColumnIndex[i] = 2 * i;
    }

    int r = evenRowIndex[random(rows / 2)];
    int c = evenColumnIndex[random(columns / 2)];

    return std::array<int, 2>{ r, c };
  }

  void defaultPattern() {

    uint64_t slice = 0b1010101010101010101010101010101010101010101010101010101010101010;

    auto func = [slice](int i, T grid[]) {
      return (i % 2 == 0 ? (T)slice : 0);
    };
    arrayMap(grid, rows, func);
  }

  void clearGrid() {

    auto func = [](int i, T grid[]) {
      return 0;
    };
    arrayMap(grid, rows, func);
  }

  void negateGrid() {

    auto func = [](int i, T grid[]) {
      return ~grid[i];
    };
    arrayMap(grid, rows, func);
    inverted = !inverted;
  }

  void pokeHole(int r, int c) {
    grid[r] = grid[r] & ~(1 << (columns - 1) - c);
  }


public:

  T* grid;
  const uint8_t rows;
  const uint8_t columns = sizeof(T) * 8;

  Maze(uint8_t rows)
    : rows(rows) {
    grid = new T[rows]{};
  }

  void generate(bool invert = true) {

    defaultPattern();
    std::array<int, 2> initialCell = randomInitialCell();
    DFS(grid, initialCell);

    if (invert) { negateGrid(); }
    pokeHole(rows - 2, columns - 1);
  }

  static void printGrid(T grid[], uint8_t rows, uint8_t columns, bool invert = false) {

    auto invf = [](bool par, bool res) {
      return par ? !res : res;
    };

    Serial.println();

    for (int i = 0; i < rows; i++) {
      Serial.print("Row ");
      Serial.print(i);
      Serial.print(": ");

      for (int j = columns - 1; j >= 0; j--) {
        Serial.print(invf(invert, (bool)(grid[i] & 1 << j)) ? "X" : "O");
        Serial.print(" ");
      }

      Serial.println();
    }
  }

  void printGrid() const {
    // don't pass in inverted since the grid will be negated with it's parity
    printGrid(grid, rows, columns, false);
  }

  ~Maze() {

    delete[] grid;
  }
};

#endif
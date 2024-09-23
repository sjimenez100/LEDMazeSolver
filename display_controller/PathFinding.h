#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <ArduinoSTL.h>
#include <stack>
#include <vector>
#include <array>

struct Node {
  array<int, 2> position;
  Node* parent = nullptr;
  bool traversable = false;

  int fCost;
  int gCost = 1000;
  int hCost;

  bool operator==(const Node& other) const {
      return position[0] == other.position[0] && position[1] == other.position[1];
  }
};

template<int rows, int columns>
vector<Node*> GetNeighbours(Node& currentNode, array<array<Node, columns>, rows>& nodeGrid){

  vector<Node*> neighbours;

  int row = currentNode.position[0];
  int column = currentNode.position[1];

  for(int r = -1; r < 2; r++){
    for(int c = -1; c < 2; c++){

      if((r == 0) && (c == 0)){
        continue;
      }

      int selRow = row + r;
      int selCol = column + c;
       
      if(selRow < 0 || selRow >= rows || selCol < 0 || selCol >= columns){
        continue;
      }

      neighbours.push_back(&nodeGrid[selRow][selCol]);
    }
  }

  return neighbours;
}

template<typename T, int rows, int columns>
vector<array<int, 2>> AStar(T grid[], array<int, 2> startPosition, array<int, 2> targetPosition){
  Serial.println("TEST2");

  array<array<Node, columns>, rows> nodeGrid;

  Serial.println("TEST3");

  for(int r = 0; r < rows; r++){
    
    for(int c = 0; c < columns; c++){
      
      Node& node = nodeGrid[r][c];
      node.traversable = !(grid[r] & (1 << (columns-1)-c));
      node.position[0] = r;
      node.position[1] = c;
    }
  }

  Serial.println("TEST4");
  vector<Node*> openSet;
  vector<Node*> closedSet;

  Node& startNode = nodeGrid[startPosition[0]][startPosition[1]];
  Node& targetNode = nodeGrid[targetPosition[0]][targetPosition[1]];

  startNode.gCost = 0;
  startNode.hCost = GetDistance(startNode, targetNode);
  startNode.fCost = startNode.gCost + startNode.hCost;

  targetNode.hCost = 0;
  targetNode.fCost = targetNode.gCost + targetNode.hCost;

  openSet.push_back(&startNode);
  Node* currentNode = &startNode;

  Serial.println("TEST5");
  
  int i = 0;
  while(!openSet.empty() && i < 500){

    int currentNodeIndex = GetLowestfCostNodeIndex(openSet);

    currentNode = openSet[currentNodeIndex];
    openSet.erase(openSet.begin() + currentNodeIndex); 
    closedSet.push_back(currentNode); 

    if(*currentNode == targetNode){
      return BuildPath(targetNode);
    }

    vector<Node*> neighbours = GetNeighbours<rows, columns>(*currentNode, nodeGrid);


    for(int n = 0; n < neighbours.size(); n++){

      Node* neighbour = neighbours[n];

      if(!neighbour->traversable || inSet(closedSet, *neighbour)){
        continue;
      }

      int newPathToNeighbour = currentNode->gCost + GetDistance(*currentNode, *neighbour);

      if(neighbour->gCost > newPathToNeighbour || !inSet(openSet, *neighbour)){

        neighbour->hCost = GetDistance(*neighbour, targetNode);
        neighbour->gCost = newPathToNeighbour;
        neighbour->fCost = neighbour->gCost + neighbour->hCost;
        neighbour->parent = currentNode;

        if(!inSet(openSet, *neighbour)){
          openSet.push_back(neighbour);
        }
      }

    }

    i++;
  }

  Serial.print("ITER:");Serial.println(i);
  return {};
}

int GetDistance(Node& nodeA, Node& nodeB){
  int distX = abs(nodeA.position[1] - nodeB.position[1]);
  int distY = abs(nodeA.position[0] - nodeB.position[0]);

  if(distX < distY){
    return 14 * distY + 10 * (distX - distY);
  }

  return 14 * distX + 10 * (distY - distX);

}

int GetLowestfCostNodeIndex(vector<Node*>& openSet) {
  int index = 0;
  Node* lowestNode = openSet[0];

  for (int i = 1; i < openSet.size(); i++) {
    Node* setNode = openSet[i];

    if (setNode->fCost < lowestNode->fCost || 
        (setNode->fCost == lowestNode->fCost && setNode->hCost < lowestNode->hCost)) {
      lowestNode = setNode;
      index = i;
    }
  }

  return index;
}


vector<array<int, 2>> BuildPath(Node& targetNode) {
  vector<array<int, 2>> path;
  Node& currentNode = targetNode;
  path.push_back(currentNode.position);

  while (currentNode.parent != nullptr) {  
    currentNode = *currentNode.parent; 
    path.push_back(currentNode.position);
  }

  reverse(path.begin(), path.end());  // Reverse the path to get it from start to target
  return path;
}

int inSet(vector<Node*>& set, Node& compNode){

  for(int n = 0; n < set.size(); n++){
    Node& node = *set[n];
    if(node == compNode){
      return true;
    }
  }

  return false;
}

#endif
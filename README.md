# LEDMazeSolver
This program is made for the MAX7291 driver that is expected to be cascaded in a "Z"-shaped pattern. Any number of drivers can be used with the firmware.

The the A* pathfinding algorithm is used to solve the maze and the maze generation is based on the iterative version of the randomized depth-first-search (DFS) algorithm.
The maze is solved iteratively, with a new maze and solution being found each time. A coroutine script is used to animate the dot that travels through the maze.


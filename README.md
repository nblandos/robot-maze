# Robot Maze
## Description\
A C program in which a robot navigates a grid filled with obstacles to collect markers and return them to its starting position.  

## To run the program
- To compile with gcc
```bash
gcc main.c robot.c graphics.c -lm
```
- To run with default values: x = 0, y = 0, direction = east
```bash
./a.out | java -jar drawapp-3.0.jar
```
- Alternatively, the robot's initial position and direction can be modified through command line arguments in the format:
```bash
./a.out x y direction | java -jar drawapp-3.0.jar
```
- For example:
```bash
./a.out 3 3 south | java -jar drawapp-3.0.jar
```
Other values such as grid size and the location of the markers and obstacles must be changed in the robot.c and robot.h files.

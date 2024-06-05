#include "graphics.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define EAST 0
#define NORTH 1
#define WEST 2
#define SOUTH 3

#define EMPTY 0
#define HOME 1
#define MARKER 2
#define OBSTACLE 3

#define GRID_ROWS 10
#define GRID_COLS 10
#define GRID_SIZE 60
#define NUM_MARKERS 4
#define NUM_OBSTACLES 6
#define NUM_DIRECTIONS 4
#define MOVE_HISTORY_SIZE 30
#define SLEEP_TIME 100

typedef struct Robot
{
    int x;
    int y;
    int direction;
    bool marker;
} Robot;

Robot initialiseRobot(int x, int y, int direction);
void initialiseGrid(int homeX, int homeY);
void initialiseDisplay(Robot *robot);
void drawGrid();
void drawGridCell(int row, int col, colour colour, bool fill);
void drawRobot(Robot *robot);
void calculateRobotPoints(int x, int y, int direction, int *xPoints, int *yPoints);
void forward(Robot *robot);
bool canMoveForward(Robot *robot);
void left(Robot *robot);
void right(Robot *robot);
void moveRobot(Robot *robot);
bool atMarker(Robot *robot);
bool isCarryingAMarker(Robot *robot);
bool markersLeft();
void pickUpMarker(Robot *robot);
void dropMarker(Robot *robot);
bool atHome(Robot *robot);
void returnHome(Robot *robot);
void recordMove(int direction);
int reverseDirection(int direction);
int directionToInt(char *directionString);
void update(Robot *robot);
void run(int homeX, int homeY, int direction);

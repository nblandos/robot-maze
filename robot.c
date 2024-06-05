#include "robot.h"

const int WIDTH = GRID_SIZE * GRID_COLS;
const int HEIGHT = GRID_SIZE * GRID_ROWS;

int markerX[NUM_MARKERS] = {6, 3, 1, 8};
int markerY[NUM_MARKERS] = {9, 5, 2, 3};
int obstacleX[NUM_OBSTACLES] = {5, 2, 0, 9, 7, 4};
int obstacleY[NUM_OBSTACLES] = {5, 9, 3, 8, 1, 0};
int grid[GRID_ROWS][GRID_COLS] = {EMPTY};
int *moveHistory = NULL;
int moveCount = 0;

Robot initialiseRobot(int x, int y, int direction)
{
    Robot robot;
    robot.x = x;
    robot.y = y;
    robot.direction = direction;
    robot.marker = false;
    return robot;
}

void initialiseGrid(int homeX, int homeY)
{
    grid[homeY][homeX] = HOME;

    for (int i = 0; i < NUM_MARKERS; i++)
    {
        grid[markerY[i]][markerX[i]] = MARKER;
    }

    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        grid[obstacleY[i]][obstacleX[i]] = OBSTACLE;
    }
}

void initialiseDisplay(Robot *robot)
{
    setWindowSize(WIDTH, HEIGHT);
    drawGrid();
    drawRobot(robot);
}

void drawGrid()
{
    background();
    clear();
    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            switch (grid[row][col])
            {
            case EMPTY:
                drawGridCell(row, col, black, false);
                break;
            case HOME:
                drawGridCell(row, col, blue, true);
                break;
            case MARKER:
                drawGridCell(row, col, red, true);
                break;
            case OBSTACLE:
                drawGridCell(row, col, black, true);
                break;
            }
        }
    }
}

void drawGridCell(int row, int col, colour colour, bool fill)
{
    setColour(colour);

    if (fill)
    {
        fillRect(col * GRID_SIZE, row * GRID_SIZE, GRID_SIZE, GRID_SIZE);
    }
    else
    {
        drawRect(col * GRID_SIZE, row * GRID_SIZE, GRID_SIZE, GRID_SIZE);
    }
}

void drawRobot(Robot *robot)
{
    foreground();
    clear();
    if (isCarryingAMarker(robot))
    {
        setColour(red);
    }
    else
    {
        setColour(green);
    }
    int numPoints = 3;
    int xPoints[numPoints];
    int yPoints[numPoints];
    calculateRobotPoints(robot->x, robot->y, robot->direction, xPoints, yPoints);
    fillPolygon(numPoints, xPoints, yPoints);
    sleep(SLEEP_TIME);
}

void calculateRobotPoints(int x, int y, int direction, int *xPoints, int *yPoints)
{
    // actual centre coordinates of the robot on the display
    int centreX = (x * GRID_SIZE) + (GRID_SIZE / 2);
    int centreY = (y * GRID_SIZE) + (GRID_SIZE / 2);
    double angle = direction * (M_PI / 2);

    for (int i = -1; i < 2; i++)
    {
        // calculate the distance from the centre of the robot to the point
        double pointDistance = (GRID_SIZE / 2) * (abs(i) * (2 / sqrt(2) - 1) + 1);
        xPoints[i + 1] = (int)(centreX + (pointDistance * cos(angle + i * M_PI * 0.75)));
        yPoints[i + 1] = (int)(centreY - (pointDistance * sin(angle + i * M_PI * 0.75)));
    }
}

void forward(Robot *robot)
{
    switch (robot->direction)
    {
    case EAST:
        robot->x++;
        break;
    case NORTH:
        robot->y--;
        break;
    case WEST:
        robot->x--;
        break;
    case SOUTH:
        robot->y++;
        break;
    }
}

bool canMoveForward(Robot *robot)
{
    switch (robot->direction)
    {
    case EAST:
        return robot->x < GRID_COLS - 1 && grid[robot->y][robot->x + 1] != OBSTACLE;
    case NORTH:
        return robot->y > 0 && grid[robot->y - 1][robot->x] != OBSTACLE;
    case WEST:
        return robot->x > 0 && grid[robot->y][robot->x - 1] != OBSTACLE;
    case SOUTH:
        return robot->y < GRID_ROWS - 1 && grid[robot->y + 1][robot->x] != OBSTACLE;
    }
    return false;
}

void left(Robot *robot)
{
    robot->direction = (robot->direction + 1) % NUM_DIRECTIONS;
}

void right(Robot *robot)
{
    robot->direction = (robot->direction + (NUM_DIRECTIONS - 1)) % NUM_DIRECTIONS;
}

void moveRobot(Robot *robot)
{
    // robot will turn if it can't move forward or randomly 40% of the time
    if (!canMoveForward(robot) || (double)rand() / RAND_MAX < 0.4)
    {
        // equal chance of turning left or right
        if ((double)rand() / RAND_MAX < 0.5)
        {
            left(robot);
        }
        else
        {
            right(robot);
        }
    }

    if (canMoveForward(robot))
    {
        forward(robot);
        recordMove(robot->direction);
    }
}

bool atMarker(Robot *robot)
{
    return grid[robot->y][robot->x] == MARKER;
}

bool isCarryingAMarker(Robot *robot)
{
    return robot->marker;
}

bool markersLeft()
{
    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            if (grid[row][col] == MARKER)
            {
                return true;
            }
        }
    }
    return false;
}

void pickUpMarker(Robot *robot)
{
    robot->marker = true;
    grid[robot->y][robot->x] = EMPTY;
    drawGrid();
}

void dropMarker(Robot *robot)
{
    robot->marker = false;
}

bool atHome(Robot *robot)
{
    return grid[robot->y][robot->x] == HOME;
}

void returnHome(Robot *robot)
{
    while (!atHome(robot))
    {
        /* uses array of directions the robot has moved in to face the opposite direction
        and move so that the robot returns home */
        moveCount--;
        int lastDirection = moveHistory[moveCount];
        while (robot->direction != reverseDirection(lastDirection))
        {
            // turns in the direction that is closest to the opposite direction
            int diff = (robot->direction - reverseDirection(lastDirection) + NUM_DIRECTIONS) % NUM_DIRECTIONS;
            if (diff == NUM_DIRECTIONS - 1)
            {
                left(robot);
            }
            else 
            {
                right(robot);
            }
            drawRobot(robot);
        }
        forward(robot);
        drawRobot(robot);
    }
    moveCount = 0;
}

void recordMove(int direction)
{
    // dynamically allocates memory for the move history array
    static int size = MOVE_HISTORY_SIZE;
    if (moveCount >= size)
    {
        size += MOVE_HISTORY_SIZE;
        moveHistory = (int *)realloc(moveHistory, sizeof(int) * size);
    }
    // stores the directions the robot has moved in
    moveHistory[moveCount] = direction;
    moveCount++;
}

int reverseDirection(int direction)
{
    return (direction + (NUM_DIRECTIONS / 2)) % NUM_DIRECTIONS;
}

int directionToInt(char *directionString)
{
    int direction;
    if (strcmp(directionString, "north") == 0)
    {
        direction = NORTH;
    }
    else if (strcmp(directionString, "south") == 0)
    {
        direction = SOUTH;
    }
    else if (strcmp(directionString, "east") == 0)
    {
        direction = EAST;
    }
    else if (strcmp(directionString, "west") == 0)
    {
        direction = WEST;
    }
    return direction;
}

void update(Robot *robot)
{
    while (markersLeft())
    {
        moveRobot(robot);
        if (atMarker(robot))
        {
            pickUpMarker(robot);
            returnHome(robot);
            dropMarker(robot);
        }
        drawRobot(robot);
    }
}

void run(int homeX, int homeY, int direction)
{
    Robot robot = initialiseRobot(homeX, homeY, direction);
    moveHistory = (int *)malloc(sizeof(int) * MOVE_HISTORY_SIZE);
    initialiseGrid(homeX, homeY);
    initialiseDisplay(&robot);
    update(&robot);
    free(moveHistory);
}
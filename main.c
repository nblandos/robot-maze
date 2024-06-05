#include "robot.h"
#include <time.h>

int main(int argc, char **argv)
{
    srand(time(NULL));

    // default values for initial robot position and direction
    int initialX = 0;
    int initialY = 0;
    char *initialDirection = "east";

    // uses command line arguments if provided
    if (argc == 4)
    {
        initialX = atoi(argv[1]);
        initialY = atoi(argv[2]);
        initialDirection = argv[3];
    }
    run(initialX, initialY, directionToInt(initialDirection));
    return 0;
}

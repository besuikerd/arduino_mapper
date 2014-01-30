#include <Arduino.h>
#include "map.h"
#include "config.h"

#ifndef PATHFINDER_H
#define PATHFINDER_H
enum Heading { NORTH=0, WEST=1, SOUTH=2, EAST=3 };
enum Direction {LEFT = 0, RIGHT = 1};

struct targetCost{
    int cost;
    int heading;
};

struct path{
    int length;
    int* directions;
};

class Pathfinder{
  public:
    Pathfinder(Map* m);
    void run(int x, int y, int heading);
    bool isTargetReachable();
    int getTargetX();
    int getTargetY();
    path getPath();
    int calculateTurningCost(int from, int to);
	int calculateTurningDirection(int from, int to);
	
    int getCostTo(int x, int y);
    int getTargetHeading(int x, int y);
    
  private:
    Map* m;
    targetCost targetCosts[MAP_SIZE][MAP_SIZE];
    int targetX;
    int targetY;
    int currentX;
    int currentY;
};
#endif
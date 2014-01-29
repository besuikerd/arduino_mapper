#include <Arduino.h>
#include "map.h"

enum Heading { NORTH=0, WEST=1, SOUTH=2, EAST=3 };

struct targetCost{
    int cost;
    int heading;
};

class Pathfinder{
  public:
    Pathfinder(Map* m);
    void run(int x, int y, int heading);
    int getTargetX();
    int getTargetY();
    
  private:
    Map* m;
    targetCost targetCosts[MAP_SIZE][MAP_SIZE];
    int targetX;
    int targetY;
    int getTargetHeading(int x, int y);
    int costTo(int x, int y);
    int turningCost(int from, int to);
};
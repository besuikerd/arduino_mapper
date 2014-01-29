#include <Arduino.h>
#include "map.h"
#include "pathfinder.h"

class Test{
  public:
    void run();
    void testMap();
    void testTurningCost();
    void testPathfinder();
    void assertEquals(int is, int ex, String msg);
};

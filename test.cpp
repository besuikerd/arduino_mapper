#include "test.h"

void Test::run(){
    this->testTurningCost();
    this->testPathfinder();
    Serial.println("Tests finished");
}

void Test::assertEquals(int is, int ex, String msg){
    if(is!=ex){
        Serial.print("Assertion error: ");
        Serial.println(msg);
    }
}

void Test::testTurningCost(){
    Serial.println("Test::testTurningCost()");
    Map m = Map(MAP_SIZE);
    Pathfinder p = Pathfinder(&m);
    
    this->assertEquals(p.calculateTurningCost(NORTH, EAST),  1, "N->E");
    this->assertEquals(p.calculateTurningCost(NORTH, WEST),  1, "N->W");
    this->assertEquals(p.calculateTurningCost(NORTH, SOUTH), 2, "N->S");
    this->assertEquals(p.calculateTurningCost(NORTH, NORTH), 0, "N->N");
    
    this->assertEquals(p.calculateTurningCost(SOUTH, EAST),  1, "S->E");
    this->assertEquals(p.calculateTurningCost(SOUTH, WEST),  1, "S->W");
    this->assertEquals(p.calculateTurningCost(SOUTH, SOUTH), 0, "S->S");
    this->assertEquals(p.calculateTurningCost(SOUTH, NORTH), 2, "S->N");
    
    this->assertEquals(p.calculateTurningCost(WEST, EAST),  2, "W->E");
    this->assertEquals(p.calculateTurningCost(WEST, WEST),  0, "W->W");
    this->assertEquals(p.calculateTurningCost(WEST, SOUTH), 1, "W->S");
    this->assertEquals(p.calculateTurningCost(WEST, NORTH), 1, "W->N");
    
    this->assertEquals(p.calculateTurningCost(EAST, EAST),  0, "E->E");
    this->assertEquals(p.calculateTurningCost(EAST, WEST),  2, "E->W");
    this->assertEquals(p.calculateTurningCost(EAST, SOUTH), 1, "E->S");
    this->assertEquals(p.calculateTurningCost(EAST, NORTH), 1, "E->N");
}

void Test::testPathfinder(){
    Serial.println("Test::testPathfinder()");
    Map m = Map(MAP_SIZE);
    Pathfinder p = Pathfinder(&m);
    
    // Set position as processed
    m.processed(2, 2);
    
    // Create a wall around the center
    m.obstacle(2,1); m.processed(2,1);
    m.obstacle(1,1); m.processed(1,1);
    m.obstacle(3,1); m.processed(3,1);
    m.obstacle(1,2); m.processed(1,2);
    m.obstacle(3,2); m.processed(3,2);
    m.obstacle(3,3); m.processed(3,3);
    
    p.run(2, 2, NORTH);
    
    this->assertEquals(p.getTargetX(), 2, "targetX");
    this->assertEquals(p.getTargetY(), 3, "targetY");
    // cost = 2x turn + 1 move = 3
    this->assertEquals(p.getCostTo(2,3), 3, "cost");
    this->assertEquals(p.getTargetHeading(2,3), SOUTH, "heading");

}
#include "test.h"

void Test::run(){
    this->testTurningCost();
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
    
    this->assertEquals(p.turningCost(NORTH, EAST),  1, "N->E");
    this->assertEquals(p.turningCost(NORTH, WEST),  1, "N->W");
    this->assertEquals(p.turningCost(NORTH, SOUTH), 2, "N->S");
    this->assertEquals(p.turningCost(NORTH, NORTH), 0, "N->N");
    
    this->assertEquals(p.turningCost(SOUTH, EAST),  1, "S->E");
    this->assertEquals(p.turningCost(SOUTH, WEST),  1, "S->W");
    this->assertEquals(p.turningCost(SOUTH, SOUTH), 0, "S->S");
    this->assertEquals(p.turningCost(SOUTH, NORTH), 2, "S->N");
    
    this->assertEquals(p.turningCost(WEST, EAST),  2, "W->E");
    this->assertEquals(p.turningCost(WEST, WEST),  0, "W->W");
    this->assertEquals(p.turningCost(WEST, SOUTH), 1, "W->S");
    this->assertEquals(p.turningCost(WEST, NORTH), 1, "W->N");
    
    this->assertEquals(p.turningCost(EAST, EAST),  0, "E->E");
    this->assertEquals(p.turningCost(EAST, WEST),  2, "E->W");
    this->assertEquals(p.turningCost(EAST, SOUTH), 1, "E->S");
    this->assertEquals(p.turningCost(EAST, NORTH), 1, "E->N");
}
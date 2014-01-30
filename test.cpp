#include "test.h"
#include "motor.h"
#include "config.h"

/** Run all tests */
void Test::run(){
    Serial.println("Tests starting...");
    this->testMap();
    this->testTurningCost();
    this->testPathfinder();
    Serial.println("Tests finished");
}

/** Assert if two integers are equal */
void Test::assertEquals(int is, int ex, String msg){
    if(is!=ex){
        Serial.print("!!! Assertion error: '");
        Serial.print(msg);
        Serial.print("' (expected=");
        Serial.print(ex);
        Serial.print(" actual=");
        Serial.print(is);
        Serial.println(")");
    }
}

/** Test the Map class, including saving/loading */
void Test::testMap(){
    Serial.println("Test::testMap()");
    int i;
    Map m0 = Map(MAP_SIZE);
    Map m1 = Map(MAP_SIZE);
    Map m2 = Map(MAP_SIZE);
    
    // Test basic functions
    m0.processed(2,2);
    this->assertEquals(m0.isProcessed(2,2), 1, "isProcessed");
    this->assertEquals(m0.isObstacle(2,2),  0, "isObstacle");
    m0.obstacle(2,2);
    this->assertEquals(m0.isProcessed(2,2), 1, "isProcessed");
    this->assertEquals(m0.isObstacle(2,2),  1, "isObstacle");
    
    // Test writing/reading
    for(i=0; i<MAP_SIZE; i++){ m1.processed(0,i); }
    for(i=0; i<MAP_SIZE; i++){ m1.processed(1,i); m1.obstacle(1,i); }
    for(i=0; i<MAP_SIZE; i++){ m1.processed(3,i); m1.obstacle(3,i); }
    for(i=0; i<MAP_SIZE; i++){ m1.processed(4,i); }
    
    writeToEEPROM(m1);
    readFromEEPROM(&m2);
    
    Serial.println("Map 1 (written to EEPROM)");
    Serial.println(m1.toString());
    Serial.println("Map 2 (read from EEPROM)");
    Serial.println(m2.toString());
    
    for(i=0; i<MAP_SIZE; i++){
        this->assertEquals(m2.isProcessed(0,i), 1, "isProcessed 0");
        this->assertEquals(m2.isObstacle(0,i),  0, "isObstacle 0");
    }
    for(i=0; i<MAP_SIZE; i++){
        this->assertEquals(m2.isProcessed(1,i), 1, "isProcessed 1");
        this->assertEquals(m2.isObstacle(1,i),  1, "isObstacle 1");
    }
    for(i=0; i<MAP_SIZE; i++){
        this->assertEquals(m2.isProcessed(2,i), 0, "isProcessed 2");
        this->assertEquals(m2.isObstacle(2,i),  0, "isObstacle 2");
    }
    for(i=0; i<MAP_SIZE; i++){
        this->assertEquals(m2.isProcessed(3,i), 1, "isProcessed 3");
        this->assertEquals(m2.isObstacle(3,i),  1, "isObstacle 3");
    }
    for(i=0; i<MAP_SIZE; i++){
        this->assertEquals(m2.isProcessed(4,i), 1, "isProcessed 4");
        this->assertEquals(m2.isObstacle(4,i),  0, "isObstacle 4");
    }
    
    
}

/** Test the calculateTurningCost method of Pathfinder */
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
    
    // -xooo
    // -xoxo
    // -xSxE
    // --x--
    // -----
    m.processed(2, 1);
    m.processed(2, 0);
    m.processed(3, 0);
    m.processed(4, 0);
    m.processed(4, 1);
    
    
    // Create a wall around the center
    m.obstacle(1,0); m.processed(1,0);
    m.obstacle(1,1); m.processed(1,1);
    m.obstacle(1,2); m.processed(1,2);
    m.obstacle(2,3); m.processed(2,3);
    m.obstacle(3,2); m.processed(3,2);
    m.obstacle(3,1); m.processed(3,1);
    
    Serial.println("Map (for pathfinding)");
    Serial.println(m.toString());
    
    p.run(2, 2, NORTH);
    
    this->assertEquals(p.getTargetX(), 4, "targetX");
    this->assertEquals(p.getTargetY(), 2, "targetY");
    this->assertEquals(p.isTargetReachable(), 1, "reachable");
    // cost = 2x turn + 6 move = 8
    this->assertEquals(p.getCostTo(4,2), 8, "cost");
    this->assertEquals(p.getTargetHeading(4,2), SOUTH, "heading");
    
    path pa = p.getPath();
    this->assertEquals(pa.length, 6, "length");
    this->assertEquals(pa.directions[0], NORTH, "directions[0]");
    this->assertEquals(pa.directions[1], NORTH, "directions[1]");
    this->assertEquals(pa.directions[2], EAST, "directions[2]");
    this->assertEquals(pa.directions[3], EAST, "directions[3]");
    this->assertEquals(pa.directions[4], SOUTH, "directions[4]");
    this->assertEquals(pa.directions[5], SOUTH, "directions[5]");

}

void Test::testRotation(){
	Motor m = Motor(DIR_L, DIR_R, PWM_L, PWM_R);
	while(true){
		if(random(2) == 1){
			m.turnLeft();
		} else{
			m.turnRight();
		}
		delay(500);
	}
}
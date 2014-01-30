#include "map.h";
#include "motor.h"
#include "pathfinder.h"
#include "test.h"
#include "bumper.h"
#include "config.h"
#include <EEPROM.h>
#include <IRremote.h>

// Map object for storing the map
Map theMap(MAP_SIZE);
// Motor object for controlling motors
Motor motor(DIR_L, DIR_R, PWM_L, PWM_R);
// Pathfinder
Pathfinder pathfinder(&theMap);

// Bumper object to check collisions
int bumperPins[2] = {BUMP_L, BUMP_R};
Bumper bumper(bumperPins);

// Position vars
int currentX = 2;
int currentY = 2;
int currentHeading = NORTH;



void setup(){
  Serial.begin(9600);
  
  if(TEST_MODE){
    Test t = Test();
    t.run();
  } else{
    theMap.processed(currentX, currentY);
    //theMap.obstacle(1,2); theMap.processed(1,2);
    //theMap.obstacle(1,3); theMap.processed(1,3);
    //readFromEEPROM(&theMap);
    setupPins();
  }
}

void setupPins(){
	pinMode(LED_FINISHED, OUTPUT);
	pinMode(BUMP_L, INPUT);
	pinMode(BUMP_R, INPUT);
	pinMode(DIR_L, OUTPUT);
	pinMode(DIR_R, OUTPUT);
	pinMode(PWM_L, OUTPUT);
	pinMode(PWM_R, OUTPUT);
}

void loop(){
    if(!TEST_MODE){
        
        
        pathfinder.run(currentX, currentY, currentHeading);
        path p = pathfinder.getPath();
        
        if(pathfinder.isTargetReachable() && p.length>0){
            boolean collision = false;
            
            if(MAPPING_DEBUG){
                Serial.print("===== Path found from (");
                Serial.print(currentX); Serial.print(","); Serial.print(currentY);
                Serial.print(") to (");
                Serial.print(pathfinder.getTargetX()); Serial.print(","); Serial.print(pathfinder.getTargetY());
                Serial.println(")");
                Serial.print("===== Length: ");
                Serial.println(p.length);
                Serial.print("===== Directions: ");
                for(int j=0; j<p.length; j++){
                    Serial.print(p.directions[j]);
                }
                Serial.println();
            }
            
            for(int i = 0; i < p.length ; i++){ //walk over path
                // Next direction in the path
                int direction = p.directions[i];
                if(MAPPING_DEBUG){ Serial.print("Moving "); Serial.println(direction); }
                
                // First turn to the correct position
                int dir = pathfinder.calculateTurningDirection(currentHeading, direction);
                int cost = pathfinder.calculateTurningCost(currentHeading, direction);
                //currentHeading = (currentHeading + (dir == LEFT ? -1 : 1) * cost + 4) % 4;
                for(int i = 0 ; i < cost ; i++){
                    switch(dir){
                        case LEFT:
                            motor.turnLeft();
                            break;
                        case RIGHT:
                            motor.turnRight();
                            break;
                    }
                }
                currentHeading = direction;
                
                // Now drive forward towards the next chunk
                int count = MOTION_COUNT; //amount of increments to check for collisions
                
                motor.forward();
                
                while(!(collision = bumper.checkCollision()) && count > 0){
                    delay(MOTION_DELAY);
                    count--;
                }
                
                motor.stop();
                
                if(collision){
                    if(MAPPING_DEBUG){ Serial.println("!!!!! Collision !!!!!!"); }
                    switch(direction){
                        case NORTH:
                            theMap.obstacle(currentX, currentY-1);
                            theMap.processed(currentX, currentY-1);
                            break;
                        case EAST:
                            theMap.obstacle(currentX+1, currentY);
                            theMap.processed(currentX+1, currentY);
                            break;
                        case SOUTH:
                            theMap.obstacle(currentX, currentY+1);
                            theMap.processed(currentX, currentY+1);
                            break;
                        case WEST:
                            theMap.obstacle(currentX-1, currentY);
                            theMap.processed(currentX-1, currentY);
                            break;
                    }
                    // Go back to the previous chunk
                    motor.back();
                    delay((MOTION_COUNT-count) * MOTION_DELAY); //return to previous position
                    motor.stop();
                    return; //do loop again, stop following the path
                }
                
                // We have arrived at a new chunk successfully. Update the current location
                switch(direction){
                    case NORTH:
                        currentY -= 1;
                        break;
                    case EAST:
                        currentX += 1;
                        break;
                    case SOUTH:
                        currentY += 1;
                        break;
                    case WEST:
                        currentX -= 1;
                        break;
                }
                if(MAPPING_DEBUG){
                    Serial.print("Successfully arrived at chunk (");
                    Serial.print(currentX); Serial.print(","); Serial.print(currentY);
                    Serial.println(")");
                }
                
                // And set this location as processed
                theMap.processed(currentX, currentY);
                
                if(MAPPING_DEBUG){
                    Serial.println("Map:");
                    Serial.println(theMap.toString());
                }
            }
            if(MAPPING_DEBUG) Serial.println("===== Path completed ====="); 
        } else{
            if(MAPPING_DEBUG) Serial.println("done!");
            finish();
        }
    }
}

void finish(){
    writeToEEPROM(theMap);

    //freeze
    while(true){
        digitalWrite(LED_FINISHED, HIGH);
        delay(1000);
        digitalWrite(LED_FINISHED, LOW);
        delay(1000);            
    }
}

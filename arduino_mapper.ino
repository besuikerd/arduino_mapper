#include "map.h";
#include "motor.h"
#include "pathfinder.h"
#include "test.h"
#include "Bumper.h"
#include "pins.h"
#include <EEPROM.h>
#include <IRremote.h>

// If 1, test methods will run instead of normal methods
#define TEST_MODE 0

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
    //readFromEEPROM(&theMap);
	setupPins();
  }
}

void setupPins(){
	pinMode(LED_FINISHED, OUTPUT);
	pinMode(BUMP_L, INPUT);
	pinMode(BUMP_R, INPUT);
}

void loop(){
	if(!TEST_MODE){
		
		
		pathfinder.run(currentX, currentY, currentHeading);
		
		if(pathfinder.isTargetReachable()){
			path p = pathfinder.getPath();
			boolean collision = false;
			
			for(int i = 0; i < p.length ; i++){ //walk over path
			
				int direction = p.directions[i];
			
				switch(direction){
				
				}
				
				
				int dir = pathfinder.calculateTurningDirection(currentHeading, direction);
				int cost = pathfinder.calculateTurningCost(currentHeading, direction);
				currentHeading = (currentHeading + (dir == LEFT ? -1 : 1) * cost + 4) % 4;
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
				
				int count = 30; //amount of increments to check for collisions
				int d = 20; //delay per check
				int total = 0; //total processed increments
				
				motor.forward();
				
				while(!(collision = bumper.checkCollision()) && count > 0){
					
					delay(d);
					count--;
					total++;
				}
				
				motor.stop();
				
				if(collision){
					theMap.obstacle(currentX, currentY);
					theMap.processed(currentX, currentY);
					motor.back();
					delay(total * d); //return to previous position
					motor.stop();
					return; //do loop again
				}
				
				theMap.processed(currentX, currentY);
				
				
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
				
				Serial.print("current: (");
				Serial.print(currentX);
				Serial.print(", ");
				Serial.print(currentY);
				Serial.println(")");
			}
		} else{
			Serial.println("done!");
			finish();
		}
	}
}

void move(boolean forward){
	

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

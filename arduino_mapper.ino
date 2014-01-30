#include "map.h";
#include "motor.h"
#include "pathfinder.h"
#include "test.h"
#include "bumper.h"
#include "pins.h"
#include <EEPROM.h>
#include <IRremote.h>

// If 1, test methods will run instead of normal methods
#define TEST_MODE 0

// Map object for storing the map
Map m(MAP_SIZE);
// Motor object for controlling motors
Motor motor = Motor(DIR_L, DIR_R, PWM_L, PWM_R);
// Pathfinder
Pathfinder pathfinder(&m);

// Bumper object to check collisions
int bumperPins[2] = {BUMP_L, BUMP_R};
Bumper bumper = Bumper(bumperPins);

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
    m.processed(currentX, currentY);
    readFromEEPROM(&m);
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
			
			while(!bumper.checkCollision()){
			}
		} else{
			finish();
		}
	}
}

void finish(){
	//freeze
	while(true){
		digitalWrite(LED_FINISHED, HIGH);
		delay(1000);
		digitalWrite(LED_FINISHED, LOW);
		delay(1000);			
	}
}

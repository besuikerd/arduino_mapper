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
  }

  while(true){
    motor.turnLeft();
    delay(1000);
    motor.turnRight();
    delay(1000);
  }
  
}



void loop(){
  if(!TEST_MODE){
    /*if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX);
        
        if(results.value != 0xFFFFFFFF){                
            for(int i=0; i<(sizeof(inputs)/sizeof(int)); i++){
                if(results.value == inputs[i]){
                    Serial.println(meanings[i]);
                }
            }
        }
        
        
        
        irrecv.resume(); // Receive the next value
    }*/
  
    Serial.println("MAP");
    Serial.println(m.toString());
    
    
    
      
    pathfinder.run(currentX, currentY, currentHeading);
    Serial.print("Target: ");
    Serial.print(pathfinder.getTargetX()); 
    Serial.print(",");
    Serial.print(pathfinder.getTargetY());
    
    while(true);
  }
}


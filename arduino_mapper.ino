#include "map.h";
#include "motor.h"
#include "pathfinder.h"
#include "test.h"
#include "Bumper.h"
#include "pins.h"
#include <EEPROM.h>
#include <IRremote.h>

// If 1, test methods will run instead of normal methods
#define TEST_MODE 1

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

// Remote control settings
int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long left_forward = 0xFF10EF;
unsigned long right_forward = 0xFF10EF;
unsigned long both_forward = 0xFF10EF;

unsigned long inputs[] = {
  0xFFA25D,
  0xFF629D,
  0xFFE21D,
  0xFF22DD,
  0xFF02FD,
  0xFFC23D,
  0xFFE01F,
  0xFFA857,
  0xFF906F,
  0xFF9867,
  0xFFB04F,

  0xFF6897, //0
  0xFF30CF, //1
  0xFF18E7, //2
  0xFF7A85, //3
  0xFF10EF, //4
  0xFF38C7, //5
  0xFF5AA5, //6
  0xFF42BD, //7
  0xFF4AB5, //8
  0xFF52AD, //9
};

char meanings[][16] = {
  "Power on/off",
  "Mode",
  "Mute/unmute",
  "Play/pauze",
  "Previous",
  "Next",
  "EQ",
  "-",
  "+",
  "Shuffle",
  "U/SD",

  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
};

void setup(){
  Serial.begin(9600);
  
  if(TEST_MODE){
    Test t = Test();
    t.run();
  }else{
    m.processed(currentX, currentY);
    m.obstacle(2,1); m.processed(2,1);
    m.obstacle(1,1); m.processed(1,1);
    m.obstacle(3,1); m.processed(3,1);
    m.obstacle(1,2); m.processed(1,2);
    m.obstacle(3,2); m.processed(3,2);
    m.obstacle(3,3); m.processed(3,3);
  }

  //map = *new Map(8);
  //motor = *new Motor();

  

  // Set up IR
  //irrecv.enableIRIn(); // Start the receiver

  //read eeprom
  //readFromEEPROM(&m);
  
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

  
  
  
  /*motor.both(p);
  Serial.println(p);
  p++;
  if(p>255) p=-255;*/
  
    delay(1000000);
  }
}


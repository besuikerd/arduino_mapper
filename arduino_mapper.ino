#include "map.h";
#include "motor.h"
#include <EEPROM.h>
#include <IRremote.h>

#define MAP_SIZE 5

enum Heading { NORTH=0, WEST=1, SOUTH=2, EAST=3 };
struct targetCost{
    int cost;
    int heading;
};

boolean b = false;

// Map object for storing the map
Map m(MAP_SIZE);
// Motor object for controlling motors
Motor motor;

int p = 0;

// Position vars
int currentX = 2;
int currentY = 2;
int currentHeading = NORTH;
int targetX = 2;
int targetY = 2;
targetCost targetCosts[MAP_SIZE][MAP_SIZE];


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
  //map = *new Map(8);
  //motor = *new Motor();

  Serial.begin(9600);

  // Set up IR
  //irrecv.enableIRIn(); // Start the receiver

  //read eeprom
  //readFromEEPROM(&m);
  m.processed(currentX, currentY);
  m.obstacle(2,1); m.processed(2,1);
  m.obstacle(1,1); m.processed(1,1);
  m.obstacle(3,1); m.processed(3,1);
  m.obstacle(1,2); m.processed(1,2);
  m.obstacle(3,2); m.processed(3,2);
  m.obstacle(3,3); m.processed(3,3);
}

/** 
* Find the closest unprocessed chunk
* Each action has a cost of 1 (action = turn or move to next chunk)
*/
void findTarget(){
    int x,y,current,n,e,w,s;
    bool changed = true;
    
    // Reset
    for(x=0; x<MAP_SIZE; x++){
        for(y=0; y<MAP_SIZE; y++){
            targetCosts[x][y].cost = 1000;
            targetCosts[x][y].heading = NORTH;
        }
    }
    // Current position
    targetCosts[currentX][currentY].cost = 0;
    targetCosts[currentX][currentY].heading = currentHeading;
    
    // Calculate path lengts
    while(changed){
        changed = false;
        for(x=0; x<MAP_SIZE; x++){
            for(y=0; y<MAP_SIZE; y++){
                if(!m.isProcessed(x,y)){
                    current = targetCosts[x][y].cost;
                    // Calculate distance by adding distance to neighbour to 1 and the number of turns needed to good
                    // from the neighbour to this point
                    n = costTo(x, y-1)+1+turningCost(getTargetHeading(x, y-1), SOUTH);
                    s = costTo(x, y+1)+1+turningCost(getTargetHeading(x, y-1), NORTH);
                    e = costTo(x+1, y)+1+turningCost(getTargetHeading(x+1, y), WEST);
                    w = costTo(x-1, y)+1+turningCost(getTargetHeading(x-1, y), EAST);
                    
                    if(n<targetCosts[x][y].cost){
                        targetCosts[x][y].cost = n;
                        targetCosts[x][y].heading = SOUTH;
                    }
                    if(s<targetCosts[x][y].cost){
                        targetCosts[x][y].cost = s;
                        targetCosts[x][y].heading = NORTH;
                    }
                    if(e<targetCosts[x][y].cost){
                        targetCosts[x][y].cost = e;
                        targetCosts[x][y].heading = WEST;
                    }
                    if(w<targetCosts[x][y].cost){
                        targetCosts[x][y].cost = w;
                        targetCosts[x][y].heading = EAST;
                    }
                    
                    if(current!=targetCosts[x][y].cost){
                        changed=true;
                        Serial.print("Found new cost to ");
                        Serial.print(x);
                        Serial.print(",");
                        Serial.print(y);
                        Serial.print(" old=");
                        Serial.print(current);
                        Serial.print(" new=");
                        Serial.println(targetCosts[x][y].cost);
                    } 
                }
            }
        }
    }
    
    // Find shortest path
    current = 1000;
    for(y=0; y<MAP_SIZE; y++){
        for(x=0; x<MAP_SIZE; x++){
            Serial.print(targetCosts[x][y].cost);
            Serial.print('|');
            Serial.print(targetCosts[x][y].heading);
            Serial.print('\t');
            if(targetCosts[x][y].cost<current && (x!=currentX || y!=currentY)){
                current = targetCosts[x][y].cost;
                targetX = x;
                targetY = y;
            }
        }
        Serial.println();
    }
}

int getTargetHeading(int x, int y){
    if(x>=0 && x<MAP_SIZE && y>=0 && y<MAP_SIZE){
        return targetCosts[x][y].heading;
    }
    return NORTH;
}

int costTo(int x, int y){
    if(x>=0 && x<MAP_SIZE && y>=0 && y<MAP_SIZE){
        return targetCosts[x][y].cost;
    }
    return 2000;
}

int turningCost(int from, int to){
    int r = abs((to - from) % 4);
    return r==3 ? 1 : r;
}

void loop(){
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
    
    /*Serial.print("N->E 1 "); Serial.println(turningCost(NORTH, EAST)); 
    Serial.print("N->W 1 "); Serial.println(turningCost(NORTH, WEST)); 
    Serial.print("N->S 2 "); Serial.println(turningCost(NORTH, SOUTH)); 
    Serial.print("N->N 0 "); Serial.println(turningCost(NORTH, NORTH)); 
    
    Serial.print("E->E 0 "); Serial.println(turningCost(EAST, EAST)); 
    Serial.print("E->W 2 "); Serial.println(turningCost(EAST, WEST)); 
    Serial.print("E->S 1 "); Serial.println(turningCost(EAST, SOUTH)); 
    Serial.print("E->N 1 "); Serial.println(turningCost(EAST, NORTH)); 
    
    Serial.print("S->E 1 "); Serial.println(turningCost(SOUTH, EAST)); 
    Serial.print("S->W 1 "); Serial.println(turningCost(SOUTH, WEST)); 
    Serial.print("S->S 0 "); Serial.println(turningCost(SOUTH, SOUTH)); 
    Serial.print("S->N 2 "); Serial.println(turningCost(SOUTH, NORTH)); 
    
    Serial.print("W->E 2 "); Serial.println(turningCost(WEST, EAST)); 
    Serial.print("W->W 0 "); Serial.println(turningCost(WEST, WEST)); 
    Serial.print("W->S 1 "); Serial.println(turningCost(WEST, SOUTH)); 
    Serial.print("W->N 1 "); Serial.println(turningCost(WEST, NORTH)); */
    
      
    findTarget();
    Serial.print("Target: ");
    Serial.print(targetX); 
    Serial.print(",");
    Serial.println(targetY);
  
  
  
  //free(&m);
  
  
  
  /*motor.both(p);
  Serial.println(p);
  p++;
  if(p>255) p=-255;*/
  
  delay(1000000);
}


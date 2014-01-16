#include "map.h";
#include "motor.h"
#include <EEPROM.h>
#include <IRremote.h>

boolean b = false;
int p = 0;

Map m = Map(8);

Motor motor;

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
  readFromEEPROM(&m);
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


  /*m.chunkAt(7, 7)->obstacle = true;
   Serial.println("BEGIN");
   Serial.println(m.toString());
   Serial.println("END");*/
  //free(&m);



  motor.both(p);
  Serial.println(p);
  p++;
  if(p>255) p=-255;

  delay(10);
}


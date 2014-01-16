#include "map.h";
#include <EEPROM.h>

boolean b = false;

//Map m = readFromEEPROM(4);
Map m = Map(8);

void setup(){
  Serial.begin(9600);
  readFromEEPROM(&m);
}

void loop(){
}

#include "map.h";
#include <EEPROM.h>

boolean b = false;

void setup(){
  Serial.begin(9600);
}



void loop(){
  Map m = *new Map(8);
  m.chunkAt(7, 7)->obstacle = true;
  Serial.println("BEGIN");
  Serial.println(m.toString());
  Serial.println("END");
  
  free(&m);
  delay(1000);
}

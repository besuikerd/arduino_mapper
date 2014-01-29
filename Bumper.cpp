#include "Bumper.h"
#include <Arduino.h>

Bumper::Bumper(int* pins){
  this->pins = pins;
  this->lastMeasurements = new int[sizeof(this->pins)];
  
  for(int i = 0 ; i < sizeof(pins) ; i++){
    pinMode(pins[i], INPUT);
  }
}

bool Bumper::checkCollision(){
  
  bool collision = false;
  
  for(int i = 0 ; i < sizeof(this->pins) ; i++){
    int measurement = digitalRead(this->pins[i]);
    if(measurement != this->lastMeasurements[i]){
      this->lastMeasurements[i] = measurement;
      if(measurement == LOW){
        collision = true;
      }
    }
  }
  return collision;
}
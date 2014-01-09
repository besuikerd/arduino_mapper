#include <EEPROM.h>

int led = 13;
int startButton = 6;
int printButton = 7;
byte running = 0;
byte buttonState = 0;

void setup() {   
    Serial.begin(9600);
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);  
    pinMode(startButton, INPUT);
    pinMode(printButton, INPUT);  
}

void loop() {
    if(digitalRead(startButton)==HIGH){
        if(bitRead(buttonState,0)==0){
            running = running==0 ? 1 : 0;
            bitWrite(buttonState,0,1);
        }
    }else{
        bitWrite(buttonState,0,0);
    }
    
    if(digitalRead(printButton)==HIGH){
        if(bitRead(buttonState,1)==0){
            Serial.println("HAHA");
            bitWrite(buttonState,1,1);
        }
    }else{
        bitWrite(buttonState,1,0);
    }

    digitalWrite(led, running==0 ? LOW : HIGH);
}

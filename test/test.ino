/*
  LiquidCrystal Library - Serial Input
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch displays text sent over the serial port 
 (e.g. from the Serial Monitor) on an attached LCD.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/LiquidCrystalSerial
 */

// include the library code:
#include <LiquidCrystal.h>
#include <IRremote.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int RECV_PIN = 10;

IRrecv irrecv(RECV_PIN);

decode_results results;

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

0xFF6897,
0xFF30CF,
0xFF18E7,
0xFF7A85,
0xFF10EF,
0xFF38C7,
0xFF5AA5,
0xFF42BD,
0xFF4AB5,
0xFF52AD,


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
    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);
    // Set up IR
    irrecv.enableIRIn(); // Start the receiver
    // initialize the serial communications:
    Serial.begin(9600);
}

void loop()
{
    if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX);
        
        if(results.value != 0xFFFFFFFF){
            lcd.clear();
            
            for(int i=0; i<(sizeof(inputs)/sizeof(int)); i++){
                if(results.value == inputs[i]){
                    lcd.setCursor(0,0);
                    lcd.print(meanings[i]);
                }
            }
            
            lcd.setCursor(0,1);
            lcd.print(results.value, HEX);
        }
        
        
        /*
            // clear the screen
            lcd.clear();
            lcd.print(results.value, HEX);
        }*/
        
       
        
        irrecv.resume(); // Receive the next value
    }
  // when characters arrive over the serial port...
  /*if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }*/
}
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int startButton = 6;
int printButton = 7;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  pinMode(startButton, INPUT);
  pinMode(printButton, INPUT);  
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, digitalRead(startButton));
}

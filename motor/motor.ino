
int count = 0;

void setup(){
    Serial.begin(9600);
    pinMode(5, OUTPUT);
}

void loop(){  
    Serial.println(count);
    analogWrite(5, count);
    count+=1;
    if(count>255) count=0;
    delay(1);
}
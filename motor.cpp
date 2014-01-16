#include "motor.h"

/*
Function    pins per Ch. A  pins per Ch. B
Direction       D12             D13
PWM	            D3              D11
Brake           D9              D8
Current Sensing A0              A1
*/

int DIRECTION_LEFT = 12;
int DIRECTION_RIGHT = 13;

int PWM_LEFT = 3;
int PWM_RIGHT = 11;

Motor::Motor(){
    pinMode(DIRECTION_LEFT, OUTPUT);
    pinMode(DIRECTION_RIGHT, OUTPUT);
    pinMode(PWM_LEFT, OUTPUT);
    pinMode(PWM_RIGHT, OUTPUT);
}

void Motor::left(int power){
    digitalWrite(DIRECTION_LEFT, power<0 ? HIGH : LOW);
    power = abs(power);
    power = power>255 ? 255 : power;
    analogWrite(PWM_LEFT, power);
}

void Motor::right(int power){
    digitalWrite(DIRECTION_RIGHT, power<0 ? HIGH : LOW);
    power = abs(power);
    power = power>255 ? 255 : power;
    analogWrite(PWM_RIGHT, power);
}

void Motor::both(int power){
    digitalWrite(DIRECTION_LEFT, power<0 ? HIGH : LOW);
    digitalWrite(DIRECTION_RIGHT, power<0 ? HIGH : LOW);
    power = abs(power);
    power = power>255 ? 255 : power;
    analogWrite(PWM_LEFT, power);
    analogWrite(PWM_RIGHT, power);
}

void Motor::stop(){
    analogWrite(PWM_LEFT, 0);
    analogWrite(PWM_RIGHT, 0);
}

#include "motor.h"

Motor::Motor(int dir_l, int dir_r, int pwm_l, int pwm_r){
    this->dir_l = dir_l;
    this->dir_r = dir_r;
    this->pwm_l = pwm_l;
    this->pwm_r = pwm_r;
  
    pinMode(this->dir_l, OUTPUT);
    pinMode(this->dir_r, OUTPUT);
    pinMode(this->pwm_l, OUTPUT);
    pinMode(this->pwm_r, OUTPUT);
}

void Motor::left(int power){
    digitalWrite(this->dir_l, power<0 ? HIGH : LOW);
    power = abs(power);
    power = power>255 ? 255 : power;
    analogWrite(this->pwm_l, power);
    digitalWrite(this->pwm_r, 0);
}

void Motor::left(){
  this->left(180);
}

void Motor::right(int power){

    digitalWrite(this->dir_r, power<0 ? HIGH : LOW);
    power = abs(power);
    power = power>255 ? 255 : power;
    analogWrite(this->pwm_l, 0);
    analogWrite(this->pwm_r, power);
}

void Motor::right(){
  this->right(180);
}

void Motor::both(int power){
    digitalWrite(this->dir_l, power<0 ? HIGH : LOW);
    digitalWrite(this->dir_r, power<0 ? HIGH : LOW);
    power = abs(power);
    power = power>255 ? 255 : power;
    Serial.println(power);
    analogWrite(this->pwm_l, power);
    analogWrite(this->pwm_r, power);
}

void Motor::forward(){
  this->both(180);
}

void Motor::back(){
  this->both(-180);
}

void Motor::stop(){
    analogWrite(this->pwm_l, 0);
    analogWrite(this->pwm_r, 0);
}

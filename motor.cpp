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
}

void Motor::left(){
  this->left(MAX_POWER);
}

void Motor::leftBack(){
  this->left(-MAX_POWER);
}

void Motor::right(int power){

    digitalWrite(this->dir_r, power<0 ? HIGH : LOW);
    power = abs(power);
    power = power>255 ? 255 : power;
    analogWrite(this->pwm_r, power);
}

void Motor::right(){
  this->right(MAX_POWER);
}

void Motor::rightBack(){
  this->right(-MAX_POWER);
}

void Motor::both(int power){
    digitalWrite(this->dir_l, power<0 ? HIGH : LOW);
    digitalWrite(this->dir_r, power<0 ? HIGH : LOW);
    power = abs(power);
    power = power>255 ? 255 : power;
    analogWrite(this->pwm_l, power + DEVIATION);
    analogWrite(this->pwm_r, power);
}

void Motor::forward(){
  this->both(MAX_POWER);
}

void Motor::back(){
  this->both(-MAX_POWER);
}

void Motor::stop(){
    analogWrite(this->pwm_l, 0);
    analogWrite(this->pwm_r, 0);
}

void Motor::turnLeft(){
  this->leftBack();
  this->right();
  delay(ROTATE_DELAY);
  this->stop();
}

void Motor::turnRight(){
  this->rightBack();
  this->left();
  delay(ROTATE_DELAY);
  this->stop();
}

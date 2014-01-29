#include <Arduino.h>

#define MAX_POWER 180

class Motor{
  public:
    Motor(int dir_l, int dir_r, int pwm_l, int pwm_r);
    void left(int power);
    void left();
    void right(int power);
    void right();
    void both(int power);
    void forward();
    void back();
    void stop();
    
  private:
    int dir_l;
    int dir_r;
    int pwm_l;
    int pwm_r;
};


#include <Arduino.h>

#define MAX_POWER 180

#define ROTATE_DELAY 500

#define DEVIATION 5

class Motor{
  public:
    Motor(int dir_l, int dir_r, int pwm_l, int pwm_r);
    void left(int power);
    void left();
    void leftBack();
    void right(int power);
    void right();
    void rightBack();
    void both(int power);
    void forward();
    void back();
    void stop();
    void turnLeft();
    void turnRight();
    
  private:
    int dir_l;
    int dir_r;
    int pwm_l;
    int pwm_r;
};


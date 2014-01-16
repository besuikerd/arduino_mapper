#include <Arduino.h>

class Motor{
  public:
    Motor();
    void left(int power);
    void right(int power);
    void both(int power);
    void stop();
};


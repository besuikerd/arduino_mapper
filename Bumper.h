class Bumper{
  public:
    Bumper(int* pins);
    bool checkCollision();
  private:
    int* pins;
    int* lastMeasurements;
};
#ifndef ship_h
#define ship_h

#include "flyingObject.h"

#define SHIP_SIZE 10

#define ROTATE_AMOUNT 6
#define THRUST_AMOUNT 0.5

#define PI 3.141592654

class Ship: public FlyingObject
{
 private:
  bool thrust;
  int angle;
  float Dx;
  float Dy;
  void calculateDirection();
 public:
  Ship();
  float getAngle() { return angle; };
  bool getThrust() { return thrust; };
  void applyThrust();
  void cutThrust();
  void rotateLeft();
  void rotateRight();
  void shipStop();
  void advance();
  void kill();
  void draw();
};

#endif /* ship_h */

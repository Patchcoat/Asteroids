/*************************************************************
 * File: flying.h
 * Author: Spencer Welch
 *
 * Description: The header for the flying class
 *************************************************************/

#ifndef BULLET_H
#define BULLET_H

#include "flyingObject.h"

#define BULLET_SPEED 5
#define BULLET_LIFE 40

class Bullet: public FlyingObject
{
 private:
  float fireSpeed;
  int life;
  Velocity angleToVector(float angle, float speed);
 public:
  Bullet();
  void advance();
  void draw();
  void fire(Point point, float angle, Velocity startSpeed);
};

#endif

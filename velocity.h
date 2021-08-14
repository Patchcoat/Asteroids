/*************************************************************
 * File: lander.cpp
 * Author: Spencer Welch
 *
 * Description: Contains the implementations of the
 *  method bodies for the velocity class.
 *************************************************************/
#ifndef VELOCITY_H
#define VELOCITY_H

class Velocity
{
 private:
  float Dx;
  float Dy;
 public:
  Velocity();
  Velocity(float inDx, float inDy);
  float getDx() { return Dx; };
  float getDy() { return Dy; };
  void setDx(float inDx);
  void setDy(float inDy);
  Velocity addX(float X);
  Velocity addY(float Y);
};

#endif

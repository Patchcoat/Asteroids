/*************************************************************
 * File: lander.cpp
 * Author: Spencer Welch
 *
 * Description: Contains the implementations of the
 *  method bodies for the velocity class.
 *************************************************************/

#include "velocity.h"

Velocity::Velocity()
{
  Dx = 0;
  Dy = 0;
}

Velocity::Velocity(float inDx, float inDy)
{
  Dx = inDx;
  Dy = inDy;
}

void Velocity::setDx(float inDx)
{
  Dx = inDx;
}

void Velocity::setDy(float inDy)
{
  Dy = inDy;
}

Velocity Velocity::addX(float X)
{
  Dx += X;
  return *this;
}

Velocity Velocity::addY(float Y)
{
  Dy += Y;
  return *this;
}

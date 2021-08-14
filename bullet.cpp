/*************************************************************
 * File: flying.cpp
 * Author: Spencer Welch
 *
 * Description: Contains the implementations of the
 *  method bodies for the flying class.
 *************************************************************/

#include "bullet.h"
#include "uiDraw.h"

#include <math.h>

Bullet::Bullet()
{
  FlyingObject();
  life = BULLET_LIFE;
}

Velocity Bullet::angleToVector(float angle, float speed)
{
  Velocity vector = Velocity();
  float rad = angle * M_PI / 180;
  float deltaX = -sin(rad);
  float deltaY = cos(rad);
  deltaY *= speed;
  deltaX *= speed;
  vector.setDy(deltaY);
  vector.setDx(deltaX);
  return vector;
}

void Bullet::advance()
{
  advanceFlying();
  if (life <= 0)
    kill();
  life--;
}

void Bullet::draw()
{
  drawDot(getPoint());
}

void Bullet::fire(Point point, float angle, Velocity startSpeed)
{
  setPoint(point);
  setVelocity(angleToVector(angle, BULLET_SPEED));
  setVelocity(Velocity(
                       getVelocity().getDx()+startSpeed.getDx(),
                       getVelocity().getDy()+startSpeed.getDy()));
  return;
}

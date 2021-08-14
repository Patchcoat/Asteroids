#include "ship.h"
#include "uiDraw.h"
#include <math.h>

Ship::Ship()
{
  setPoint(Point(0,0));
  setVelocity(Velocity(0,0));
  thrust = THRUST_AMOUNT;
  angle = 0;
  setRadius(10);
  thrust = false;
}

void Ship::calculateDirection()
{
  float rad = angle * PI / 180;
  Dx = -sin(rad);
  Dy = cos(rad);
}

void Ship::applyThrust()
{
  if (!isAlive())
    return;
  thrust = true;

  calculateDirection();
  float Dx = this->Dx * THRUST_AMOUNT;
  float Dy = this->Dy * THRUST_AMOUNT;
  Velocity velocity = getVelocity();
  velocity.setDx(getVelocity().getDx() + Dx);
  velocity.setDy(getVelocity().getDy() + Dy);
  setVelocity(velocity);
}

void Ship::cutThrust()
{
  thrust = false;
}

void Ship::rotateLeft()
{
  angle += ROTATE_AMOUNT;
}

void Ship::rotateRight()
{
  angle -= ROTATE_AMOUNT;
}

void Ship::shipStop()
{
  //setVelocity(Velocity());
}

void Ship::advance()
{
  advanceFlying();
}

void Ship::kill()
{
  FlyingObject::kill();
}

void Ship::draw()
{
  if (!isAlive())
  {
    drawText(Point(), "GAME OVER");
    return;
  }
  for (int i = 0; i < 8; i++)
    drawShip(getEdgePoints()[i], angle, thrust);
  drawShip(getPoint(), angle, thrust);
}

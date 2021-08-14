#include "flyingObject.h"

FlyingObject::FlyingObject()
{
  velocity = Velocity();
  point = Point();
  alive = true;
  radius = 0;
}

FlyingObject::FlyingObject(Point point)
{
  velocity = Velocity();
  this->point = point;
  alive = true;
  radius = 0;
}

bool FlyingObject::isAlive()
{
  return alive;
}

void FlyingObject::setPoint(Point point)
{
  this->point = point;
}

void FlyingObject::setVelocity(Velocity velocity)
{
  this->velocity = velocity;
}

void FlyingObject::setEdges(float left, float right, float top, float bottom)
{
  this->left = left;
  this->right = right;
  this->top = top;
  this->bottom = bottom;
}

void FlyingObject::refreshEdgePoints()
{
  for (int i = 0; i < 8; i++)
  {
    edgePoints[i] = point;
    if (i != 2 && i != 5)
      edgePoints[i].addY((i % 2 ? top : bottom) * 2);
    if (i != 1 && i != 6)
      edgePoints[i].addX((i >= 4 ? left : right) * 2);
  }
}

void FlyingObject::setRadius(float radius)
{
  this->radius = radius;
}

void FlyingObject::kill()
{
  alive = false;
}

void FlyingObject::advanceFlying()
{
  point.addX(velocity.getDx());
  point.addY(velocity.getDy());
  // When the ship reaches one side of the screen teleport
  // to the other side
  if (point.getX() >= right)
  {
    point.setX(left + 0.1);
  }
  if (point.getX() <= left)
  {
    point.setX(right - 0.1);
  }
  if (point.getY() >= top)
  {
    point.setY(bottom + 0.1);
  }
  if (point.getY() <= bottom)
  {
    point.setY(top - 0.1);
  }
  refreshEdgePoints();
}

void FlyingObject::draw()
{
  return;
}

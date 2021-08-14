#ifndef flyingObject_h
#define flyingObject_h

#include "point.h"
#include "velocity.h"

class FlyingObject
{
 private:
  Velocity velocity;
  Point point;
  Point edgePoints[8];

  bool alive;
  float radius;
  void refreshEdgePoints();
 public:
  float left, right, top, bottom;
  FlyingObject();
  FlyingObject(Point point);
  Point getPoint() { return point; };
  Point *getEdgePoints() { return edgePoints; };
  Velocity getVelocity() { return velocity; };
  Point getConstPoint() const { return point; };
  Velocity getConstVelocity() const { return velocity; };
  float getRadius() const { return radius; };
  bool isAlive ();
  void setPoint (Point point);
  void setVelocity(Velocity velocity);
  void setRadius(float radius);
  void setEdges(float left, float right, float top, float bottom);
  void kill();
  void advanceFlying();
  void draw();
};



#endif /* flyingObject_h */

#ifndef ASTEROID_H
#define ASTEROID_H

#include "flyingObject.h"
#include "point.h"

#include <vector>

enum AsteroidSize
{
  large,
  medium,
  small
};

class Asteroid: public FlyingObject
{
 private:
  float rotation;
  float spinSpeed;
  AsteroidSize size;
 public:
  Asteroid();
  Asteroid(Point spawnPoint, Velocity spawnVelocity, AsteroidSize size);
  void advance();
  void draw();
  std::vector<Asteroid> hit();
};

#endif

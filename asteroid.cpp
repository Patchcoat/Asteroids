#include "asteroid.h"
#include "point.h"
#include "velocity.h"
#include "uiDraw.h"

Asteroid::Asteroid()
{
  setPoint(Point());
  setVelocity(Velocity());
  rotation = random(0, 359);
  setRadius(16);
  spinSpeed = 2;
  left = -200;
  right = 200;
  top = 200;
  bottom = -200;
  size = large;
}

Asteroid::Asteroid(Point spawnPoint, Velocity spawnVelocity, AsteroidSize size)
{
  setPoint(spawnPoint);
  setVelocity(spawnVelocity);
  this->size = size;
  rotation = random(0, 359);
  switch (size)
  {
  case AsteroidSize::large:
    spinSpeed = 2;
    setRadius(16);
    break;
  case AsteroidSize::medium:
    spinSpeed = 5;
    setRadius(8);
    break;
  case AsteroidSize::small:
    spinSpeed = 10;
    setRadius(4);
    break;
  }
  left = -200;
  right = 200;
  top = 200;
  bottom = -200;
}

std::vector<Asteroid> Asteroid::hit()
{
  std::vector<Asteroid> asteroids;
  switch (size)
  {
  case AsteroidSize::large:
    asteroids.push_back(Asteroid(getPoint(), getVelocity().addY(1), AsteroidSize::medium));
    asteroids.push_back(Asteroid(getPoint(), getVelocity().addY(-1), AsteroidSize::medium));
    asteroids.push_back(Asteroid(getPoint(), getVelocity().addX(-1), AsteroidSize::small));
    kill();
    break;
  case AsteroidSize::medium:
    asteroids.push_back(Asteroid(getPoint(), getVelocity().addX(3), AsteroidSize::small));
    asteroids.push_back(Asteroid(getPoint(), getVelocity().addX(-3), AsteroidSize::small));
    kill();
    break;
  case AsteroidSize::small:
    kill();
    break;
  }

  return asteroids;
}

void Asteroid::advance()
{
  advanceFlying();
  rotation += spinSpeed;
}

void Asteroid::draw()
{
  switch (size)
  {
  case AsteroidSize::large:
    for (int i = 0; i < 8; i++)
      drawLargeAsteroid(getEdgePoints()[i], rotation);
    drawLargeAsteroid(getPoint(), rotation);
    break;
  case AsteroidSize::medium:
    for (int i = 0; i < 8; i++)
      drawMediumAsteroid(getEdgePoints()[i], rotation);
    drawMediumAsteroid(getPoint(), rotation);
    break;
  case AsteroidSize::small:
    for (int i = 0; i < 8; i++)
      drawSmallAsteroid(getEdgePoints()[i], rotation);
    drawSmallAsteroid(getPoint(), rotation);
    break;
  }
}

/*********************************************************************
 * File: game.cpp
 * Description: Contains the implementaiton of the game class
 *  methods.
 *
 *********************************************************************/


#include "game.h"
#include "flyingObject.h"
#include "ship.h"

// These are needed for the getClosestDistance function...
#include <limits>
#include <algorithm>
using namespace std;

#define OFF_SCREEN_BORDER_AMOUNT 5

/***************************************
 * GAME CONSTRUCTOR
 ***************************************/
Game :: Game(Point tl, Point br)
  : topLeft(tl), bottomRight(br)
{
  // Set up the initial conditions of the game
  paused = false;
  upHold = false;
  downHold = false;
  enterHold = false;
  score = 0;
  masterVolume = 1;
  effectVolume = 0.5;
  musicVolume = 0.5;
  ship = Ship();
  pause = Pause();
  ship.setEdges(topLeft.getX(), bottomRight.getX(), topLeft.getY(), bottomRight.getY());
  for (int i = 0; i < 5; i++)
  {
    Point point = Point(random(-100.0, 100.0), random(-100.0, 100.0));
    Velocity velocity = Velocity(random(-1.0, 1.0), random(-1.0, 1.0));
    //force the asteroid to spawn at an edge.
    int edge = random(0, 4);
    switch(edge)
    {
    case 0:
      point.setX(topLeft.getX());
      break;
    case 1:
      point.setX(bottomRight.getX());
      break;
    case 2:
      point.setY(topLeft.getY());
      break;
    case 3:
      point.setY(bottomRight.getY());
      break;
    }
    asteroids.push_back(Asteroid(point, velocity, large));
  }

  audioEngine.Init();
  audioEngine.LoadSound("data/Satellite One by Purple Motion.ogg", false, true, true);
  audioEngine.LoadSound("data/BeaT-Celestial Fantasia.ogg", false, true, true);
  audioEngine.LoadSound("data/Laser_shoot.ogg");
  audioEngine.LoadSound("data/Explosion.ogg");
  audioEngine.LoadSound("data/Hit_hurt.ogg");
  audioEngine.LoadSound("data/Blip.ogg");
  audioEngine.LoadSound("data/Select.ogg");
  audioEngine.LoadSound("data/Engine.ogg", false, true);
  musicChannel = audioEngine.PlaySound("data/Satellite One by Purple Motion.ogg", {0, 0, 0}, audioEngine.VolumeTodB(0.5));
  audioEngine.SetChannelMute(musicChannel, true);
  dsp_LowPass = audioEngine.LowPassOnChannel(musicChannel, 500);
  dsp_LowPass->setBypass(true);
  rocketNoise = audioEngine.PlaySound("data/Engine.ogg",{0,0,0}, 5);
  audioEngine.SetChannelMute(rocketNoise, true);
  paused = true;
  start = Start();
  start.setTime(0);
  start.setStartMusic(audioEngine.PlaySound("data/BeaT-Celestial Fantasia.ogg"));
}

/****************************************
 * GAME DESTRUCTOR
 ****************************************/
Game :: ~Game()
{
  audioEngine.Shutdown();
  asteroids.clear();
  bullets.clear();
  cleanUpZombies();
}

/***************************************
 * GAME :: ADVANCE
 * advance the game one unit of time
 ***************************************/
void Game :: advance()
{
  if (!paused)
  {
    advanceAsteroids();
    advanceBullets();
    ship.advance();
    if (ship.getThrust())
    {
      audioEngine.SetChannelMute(rocketNoise, false);
    }
    else
    {
      audioEngine.SetChannelMute(rocketNoise, true);
    }
    handleCollisions();
    cleanUpZombies();
  }
  if (paused)
  {
    if (start.getOnStart())
      start.advance();
    pause.advance();
  }
  audioEngine.advance();
}

/***************************************
 * GAME :: ADVANCE BULLETS
 * Go through each bullet and advance it.
 ***************************************/
void Game :: advanceBullets()
{
  // Move each of the bullets forward if it is alive
  for (int i = 0; i < bullets.size(); i++)
  {
    if (bullets[i].isAlive())
    {
      // this bullet is alive, so tell it to move forward
      bullets[i].advance();

      if (!isOnScreen(bullets[i].getPoint()))
      {
        // the bullet has left the screen
        bullets[i].kill();
      }

    }
  }
}

void Game :: restart()
{
  score = 0;
  ship = Ship();
  ship.setEdges(topLeft.getX(), bottomRight.getX(), topLeft.getY(), bottomRight.getY());
  asteroids.clear();
  bullets.clear();
  for (int i = 0; i < 5; i++)
  {
    Point point = Point(random(-100.0, 100.0), random(-100.0, 100.0));
    Velocity velocity = Velocity(random(-1.0, 1.0), random(-1.0, 1.0));
    //force the asteroid to spawn at an edge.
    int edge = random(0, 4);
    switch(edge)
    {
    case 0:
      point.setX(topLeft.getX());
      break;
    case 1:
      point.setX(bottomRight.getX());
      break;
    case 2:
      point.setY(topLeft.getY());
      break;
    case 3:
      point.setY(bottomRight.getY());
      break;
    }
    asteroids.push_back(Asteroid(point, velocity, large));
  }
  cleanUpZombies();
  advanceAsteroids();
}

/***************************************
 * GAME :: SELECT
 * preforms an action based on a pause
 * menu selection
 ***************************************/
void Game :: select(int sel)
{
  if (start.getOnStart())
  {
    switch(sel)
    {
    case 0:// Resume
      paused = false;
      pause.setVisible(paused);
      dsp_LowPass->setBypass(true);
      start.setOnStart(false);
      musicChannel = audioEngine.PlaySound("data/Satellite One by Purple Motion.ogg", {0, 0, 0}, audioEngine.VolumeTodB(0.5));
      audioEngine.SetChannelMute(start.getStartMusic(), true);
      break;
    case 1:// Quit
      exit(0);
      break;
    }
    return;
  }
  switch(sel)
  {
  case -1:
    audioEngine.PlaySound("data/Select.ogg");
    break;
  case 0:// Resume
    paused = false;
    pause.setVisible(paused);
    dsp_LowPass->setBypass(true);
    break;
  case 1:// Restart
    paused = false;
    pause.setVisible(paused);
    restart();
    dsp_LowPass->setBypass(true);
    break;
  case 2:// sound
    audioEngine.PlaySound("data/Select.ogg");
    pause.soundWindow(true);
    break;
  case 3:// Keybind
    audioEngine.PlaySound("data/Select.ogg");
    pause.keyBindWindow(true);
    break;
  case 4:// Quit
    exit(0);
    break;
  }
}

void Game :: advanceAsteroids()
{
  if (asteroids.size() < 5)
  {
    Point point = Point(random(-100.0, 100.0), random(-100.0, 100.0));
    Velocity velocity = Velocity(random(-1.0, 1.0), random(-1.0, 1.0));
    //force the asteroid to spawn at an edge.
    int edge = random(0, 4);
    switch(edge)
      {
      case 0:
        point.setX(topLeft.getX());
        break;
      case 1:
        point.setX(bottomRight.getX());
        break;
      case 2:
        point.setY(topLeft.getY());
        break;
      case 3:
        point.setY(bottomRight.getY());
        break;
      }
    asteroids.push_back(Asteroid(point, velocity, large));
  }
  for (int i = 0; i < asteroids.size(); i++)
  {
    asteroids[i].advance();
  }
}

/**************************************************************************
 * GAME :: IS ON SCREEN
 * Determines if a given point is on the screen.
 **************************************************************************/
bool Game :: isOnScreen(const Point & point)
{
  return (point.getX() >= topLeft.getX() - OFF_SCREEN_BORDER_AMOUNT
          && point.getX() <= bottomRight.getX() + OFF_SCREEN_BORDER_AMOUNT
          && point.getY() >= bottomRight.getY() - OFF_SCREEN_BORDER_AMOUNT
          && point.getY() <= topLeft.getY() + OFF_SCREEN_BORDER_AMOUNT);
}

/**********************************************************
 * Function: GET CLOSEST DISTANCE
 * Description: Determine how close these two objects will
 *   get in between the frames.
 **********************************************************/
float Game :: getClosestDistance(const FlyingObject &obj1, const FlyingObject &obj2) const
{
   // find the maximum distance traveled
   float dMax = max(abs(obj1.getConstVelocity().getDx()), abs(obj1.getConstVelocity().getDy()));
   dMax = max(dMax, abs(obj2.getConstVelocity().getDx()));
   dMax = max(dMax, abs(obj2.getConstVelocity().getDy()));
   dMax = max(dMax, 0.1f); // when dx and dy are 0.0. Go through the loop once.

   float distMin = std::numeric_limits<float>::max();
   for (float i = 0.0; i <= dMax; i++)
   {
     Point point1(obj1.getConstPoint().getX() + (obj1.getConstVelocity().getDx() * i / dMax),
                  obj1.getConstPoint().getY() + (obj1.getConstVelocity().getDy() * i / dMax));
     Point point2(obj2.getConstPoint().getX() + (obj2.getConstVelocity().getDx() * i / dMax),
                  obj2.getConstPoint().getY() + (obj2.getConstVelocity().getDy() * i / dMax));

      float xDiff = point1.getX() - point2.getX();
      float yDiff = point1.getY() - point2.getY();

      float distSquared = (xDiff * xDiff) +(yDiff * yDiff);

      distMin = min(distMin, distSquared);
   }

   return sqrt(distMin);
}

/**********************************************************
 * Function: HANDLE COLLISIONS
 * Check for a collision between two flying objects
 **********************************************************/
void Game :: handleCollisions()
{
  // now check for a hit (if it is close enough to any live bullets)

  for (int i = 0; i < asteroids.size(); i++)
  {
    if (asteroids[i].isAlive())
    {
      for (int j = 0; j < bullets.size(); j++)
      {
        if (bullets[j].isAlive())
        {
          // for the real asteroid
          if (getClosestDistance(bullets[j], asteroids[i]) <= asteroids[i].getRadius())
          {
            std::vector<Asteroid> newAsteroids = asteroids[i].hit();
            asteroids.insert(asteroids.end(), newAsteroids.begin(), newAsteroids.end());

            bullets[j].kill();
            audioEngine.PlaySound("data/Explosion.ogg", {0,0,0}, effectVolume);
            // avoid the asteroids suddenly spawning in the center of the screen
            score += 5;
            advanceAsteroids();
          }
          // for the edge asteroids
          for (int k = 0; k < 8; k++)
          {
            FlyingObject obj = FlyingObject(asteroids[i].getEdgePoints()[k]);
            float distance = getClosestDistance(bullets[j], obj);
            if (distance <= asteroids[i].getRadius())
            {
              std::vector<Asteroid> newAsteroids = asteroids[i].hit();
              asteroids.insert(asteroids.end(), newAsteroids.begin(), newAsteroids.end());

              bullets[j].kill();
              audioEngine.PlaySound("data/Explosion.ogg", {0,0,0}, effectVolume);
              // avoid the asteroids suddenly spawning in the center of the screen
              score += 5;
              advanceAsteroids();
            }
          }
        } // if the bullet is alive
      } // for bullets
      if (ship.isAlive())
      {
        // for the real asteroid
        if (getClosestDistance(ship, asteroids[i]) <= asteroids[i].getRadius() + ship.getRadius())
        {
          asteroids[i].hit();

          ship.kill();
          audioEngine.PlaySound("data/Hit_hurt.ogg", {0, 0, 0}, effectVolume);
        }
        // for the edge asteroids
        // the edge ships don't have collision, this is
        // by design
        for (int k = 0; k < 8; k++)
        {
          FlyingObject obj = FlyingObject(asteroids[i].getEdgePoints()[k]);
          float distance = getClosestDistance(ship, obj);
          if (distance <= asteroids[i].getRadius() + ship.getRadius())
          {
            asteroids[i].hit();

            ship.kill();
            audioEngine.PlaySound("data/Hit_hurt.ogg", {0, 0, 0}, effectVolume);
          }
        }
      }
    } // if asteroid is alive
  } // for asteroid
  return;
}

/**************************************************************************
 * GAME :: CLEAN UP ZOMBIES
 * Remove any dead objects (take bullets out of the list, deallocate bird)
 **************************************************************************/
void Game :: cleanUpZombies()
{
  // Look for dead bullets
  vector<Bullet>::iterator bulletIt = bullets.begin();
  while (bulletIt != bullets.end())
  {
    Bullet bullet = *bulletIt;
    // Asteroids Hint:
    // If we had a list of pointers, we would need this line instead:
    //Bullet* pBullet = *bulletIt;
    if (!bullet.isAlive())
    {
      // If we had a list of pointers, we would need to delete the memory here...


      // remove from list and advance
      bulletIt = bullets.erase(bulletIt);
    }
    else
    {
      bulletIt++; // advance
    }
  }
  // Look for dead bullets
  vector<Asteroid>::iterator asteroidIt = asteroids.begin();
  while (asteroidIt != asteroids.end())
  {
    Asteroid asteroid = *asteroidIt;
    // Asteroids Hint:
    // If we had a list of pointers, we would need this line instead:
    //Bullet* pBullet = *bulletIt;
    if (!asteroid.isAlive())
    {
      // If we had a list of pointers, we would need to delete the memory here...


      // remove from list and advance
      asteroidIt = asteroids.erase(asteroidIt);
    }
    else
    {
      asteroidIt++; // advance
    }
  }
}

/***************************************
 * GAME :: HANDLE INPUT
 * accept input from the user
 ***************************************/
void Game :: handleInput(const Interface & ui)
{
   if (ui.isP())
   {
     paused = !paused;
     pause.setVisible(paused);
     pause.setUI(ui);
     if (paused)
     {
       dsp_LowPass->setBypass(false);
       audioEngine.SetChannelMute(rocketNoise, true);
     }
     else
     {
       dsp_LowPass->setBypass(true);
     }
   }
   if (!paused)
   {
      // Change the direction of the rifle
      if (ui.isLeft())
      {
        ship.rotateLeft();
      }

      if (ui.isRight())
      {
        ship.rotateRight();
      }

      if (ui.isUp())
      {
        ship.applyThrust();
      }
      else if (ui.isUp() == false)
      {
        ship.cutThrust();
      }

      if (ui.isDown())
      {
        ship.shipStop();
      }
      // Check for "Spacebar
      if (ui.isSpace())
      {
        if (!ship.isAlive())
          return;
         Bullet newBullet = Bullet();
         newBullet.setEdges(-200, 200, 200, -200);
         newBullet.fire(ship.getPoint(), ship.getAngle(), ship.getVelocity());
         bullets.push_back(newBullet);

         audioEngine.PlaySound("data/Laser_shoot.ogg", {0, 0, 0}, effectVolume);
      }
   }
   if (paused)
   {
      if (ui.isUp())
      {
         if (!upHold)
         {
            if (start.getOnStart())
              start.moveUp();
            else
              pause.moveUp();
            upHold = true;
            audioEngine.PlaySound("data/Blip.ogg");
         }
      }
      else
      {
         upHold = false;
      }
      if (ui.isDown())
      {
        if (!downHold)
        {
           if (start.getOnStart())
             start.moveDown();
           else
           pause.moveDown();
           downHold = true;
           audioEngine.PlaySound("data/Blip.ogg");
        }
      }
      else
      {
         downHold = false;
      }
      if (ui.isLeft())
      {
        if (!leftHold)
        {
          pause.moveLeft();
          leftHold = true;
          audioEngine.PlaySound("data/Blip.ogg");
        }
      }
      else
      {
        leftHold = false;
      }
      if (ui.isRight())
      {
        if (!rightHold)
        {
          pause.moveRight();
          rightHold = true;
          audioEngine.PlaySound("data/Blip.ogg");
        }
      }
      else
      {
        rightHold = false;
      }
      if (ui.isEnter())
      {
        if (!enterHold)
        {
           enterHold = true;
           if (start.getOnStart())
             select(start.select());
           else
             select(pause.select());
        }
      }
      else
      {
         enterHold = false;
      }

      if (pause.getSoundVisible())
      {
        masterVolume = pause.getSound().getMasterVolume();
        musicVolume = pause.getSound().getMusicVolume();
        musicVolume *= masterVolume;
        musicVolume = audioEngine.VolumeTodB(musicVolume);
        audioEngine.SetChannelVolume(musicChannel, musicVolume);
        effectVolume = pause.getSound().getEffectVolume();
        effectVolume *= masterVolume;
        effectVolume = audioEngine.VolumeTodB(effectVolume);
        audioEngine.SetChannelVolume(rocketNoise, effectVolume * 5);
      }
   }
}

void Game :: draw(const Interface & ui)
{
  if (start.getOnStart())
  {
    start.draw();
    return;
  }
  ship.draw();
  for (Asteroid asteroid : asteroids)
  {
    //cout << asteroid.getPoint().getX();
    asteroid.draw();
  }
  // draw the bullets, if they are alive
  for (int i = 0; i < bullets.size(); i++)
  {
    if (bullets[i].isAlive())
    {
      bullets[i].draw();
    }
  }

  Point fuelLocation;
  fuelLocation.setX(topLeft.getX() + 5);
  fuelLocation.setY(topLeft.getY() - 5);

  drawNumber(fuelLocation, score);

  pause.draw();
}

/*********************************************************************
 * File: game.h
 * Description: Defines the game class for Asteroids
 *
 *********************************************************************/

#ifndef GAME_H
#define GAME_H

#include "uiDraw.h"
#include "uiInteract.h"
#include "audioEngine.h"
#include "start.h"
#include "point.h"
#include "velocity.h"
#include "flyingObject.h"
#include "ship.h"
#include "bullet.h"
#include "asteroid.h"
#include "pause.h"
#include "soundOptions.h"

#include <vector>

/*****************************************
 * GAME
 * The main game class containing all the state
 *****************************************/
class Game
{
public:
   /*********************************************
    * Constructor
    * Initializes the game
    *********************************************/
   Game(Point tl, Point br);
   ~Game();

   /*********************************************
    * Function: handleInput
    * Description: Takes actions according to whatever
    *  keys the user has pressed.
    *********************************************/
   void handleInput(const Interface & ui);
   /*********************************************
    * Function: advance
    * Description: Move everything forward one
    *  step in time.
    *********************************************/
   void advance();

   /*********************************************
    * Function: draw
    * Description: draws everything for the game.
    *********************************************/
   void draw(const Interface & ui);

   /*********************************************
    * Function: getClosestDistance
    * Description: takes in two objects finds the
    *  distance between them;
    *********************************************/
   float getClosestDistance(const FlyingObject &obj1, const FlyingObject &obj2) const;

private:
   // pause menu methods
   Pause pause;
   bool paused;
   bool upHold;
   bool downHold;
   bool leftHold;
   bool rightHold;
   bool enterHold;
   // start menu
   Start start;
   // sound settings
   float masterVolume;
   float effectVolume;
   float musicVolume;
   // The coordinates of the screen
   Point topLeft;
   Point bottomRight;

   int score;

   std::vector<Asteroid> asteroids;
   std::vector<Bullet> bullets;

   Ship ship;

   void advanceAsteroids();
   void advanceBullets();

   AudioEngine audioEngine;
   int musicChannel;
   int rocketNoise;
   FMOD::DSP *dsp_LowPass;

   /*********************************************
    * Function: select
    * Description: preforms an action based on a
    * pause menu selection.
    *********************************************/
   void select(int sel);

   /*************************************************
    * Private methods to help with the game logic.
    *************************************************/
   bool isOnScreen(const Point & point);
   void handleCollisions();
   void restart();
   void cleanUpZombies();
};

#endif /* GAME_H */

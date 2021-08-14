/*************************************************************
 * File: pause.h
 * Author: Spencer Welch
 *
 * Description: Contains the definition of the pause class.
 *************************************************************/
#ifndef PAUSE_H
#define PAUSE_H

#include "keyBind.h"
#include "soundOptions.h"
#include "uiInteract.h"
#include <string>

class Pause
{
 private:
  KeyBind keyBind;
  SoundOptions soundOption;
  Interface ui;
  int selection;
  int maxSelect;
  int visible;
  int selRot;
  bool keyBinding;
  bool soundOptions;
 public:
  Pause();
  SoundOptions getSound() { return soundOption; };
  bool getSoundVisible() { return soundOptions; };
  void setVisible(bool visible);
  void setUI(const Interface & ui);
  void moveUp();
  void moveDown();
  void moveLeft();
  void moveRight();
  void keyBindWindow(bool open);
  void soundWindow(bool open);
  int select();
  void advance();
  void draw();
};

#endif

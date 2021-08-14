/*************************************************************
 * File: keyBind.h
 * Author: Spencer Welch
 *
 * Description: Contains the definition of the key bind class.
 *************************************************************/
#ifndef KEYBIND_H
#define KEYBIND_H

#include "uiInteract.h"

#include <string>

class KeyBind
{
 private:
  Interface ui;
  int selection;
  int maxSelect;
  int selRot;
  char upKey;
  char downKey;
  char leftKey;
  char rightKey;
  char spaceKey;
  std::string upString;
  std::string downString;
  std::string leftString;
  std::string rightString;
  std::string spaceString;
  bool visible;
  bool changingKeys;
 public:
  KeyBind();
  void setVisible(bool visible);
  void setUI(const Interface & ui);
  std::string bindToStr(KEYBIND bind);
  void setUIBinds();
  void moveUp();
  void moveDown();
  int select();
  void advance();
  void draw();
};

#endif

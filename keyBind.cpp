/*************************************************************
 * File: keyBind.cpp
 * Author: Spencer Welch
 *
 * Description: Contains the implementations of the
 *  method bodies for the key bind class.
 *************************************************************/

#include "keyBind.h"
#include "uiDraw.h"

#include <string>

KeyBind::KeyBind()
{
  selection = 0;
  maxSelect = 5;
  selRot = 0;
  visible = false;
  changingKeys = false;
  upKey = 'w';
  downKey = 's';
  rightKey = 'd';
  leftKey = 'a';
  spaceKey = ' ';
  upString = "W";
  downString = "S";
  rightString = "D";
  leftString = "A";
  spaceString = "Space";
}

void KeyBind::setVisible(bool visible)
{
  this->visible = visible;
  if (!visible)
  {
    selection = 0;
  }
}

void KeyBind::setUI(const Interface & ui)
{
  this->ui = ui;
}

std::string KeyBind::bindToStr(KEYBIND bind)
{
  if (bind.spec)
  {
    switch(bind.key)
    {
    case 100:
      return "Left Key";
      break;
    case 101:
      return "Up Key";
      break;
    case 102:
      return "Right Key";
      break;
    case 103:
      return "Down Key";
      break;
    default:
      break;
    }
  }
  else
  {
    char key[2];
    if (bind.key >= 'a' && bind.key <= 'z')
      key[0] = bind.key - ' ';// capitalize
    if (bind.key == ' ')
      return "Space";
    key[1] = '\0';
    return std::string(key);
  }
  return "";
}

void KeyBind::setUIBinds()
{
  upString = bindToStr(ui.getUpKey());
  downString = bindToStr(ui.getDownKey());
  leftString = bindToStr(ui.getLeftKey());
  rightString = bindToStr(ui.getRightKey());
  spaceString = bindToStr(ui.getSpaceKey());
}

void KeyBind::moveUp()
{
  selection--;
  if (selection < 0)
    selection = maxSelect;
}

void KeyBind::moveDown()
{
  selection++;
  if (selection > maxSelect)
    selection = 0;
}

int KeyBind::select()
{
  if (changingKeys)
  {
    setUIBinds();
    changingKeys = false;
    return -1;
  }
  switch(selection)
  {
  case 0:
    return 0;
    break;
  case 1:
    upString = "press a key";
    changingKeys = true;
    break;
  case 2:
    downString = "press a key";
    changingKeys = true;
    break;
  case 3:
    leftString = "press a key";
    changingKeys = true;
    break;
  case 4:
    rightString = "press a key";
    changingKeys = true;
    break;
  case 5:
    spaceString = "press a key";
    changingKeys = true;
    break;
  }
  ui.triggerChangeKey(selection - 1);
  return selection;
}

void KeyBind::advance()
{
  selRot += 10;
  if (selRot > 360)
    selRot = 0;
}

void KeyBind::draw()
{
  if (!visible)
    return;
  const char * upStr = upString.c_str();
  const char * downStr = downString.c_str();
  const char * leftStr = leftString.c_str();
  const char * rightStr = rightString.c_str();
  const char * spaceStr = spaceString.c_str();
  drawScrnWdthRect(Point(0, -5), 75, 0);
  drawText(Point(-5, 24), "Back");
  drawText(Point(-5, 12), "Up");
  drawText(Point(25, 12), upStr);
  drawText(Point(-5, 0), "Down");
  drawText(Point(25, 0), downStr);
  drawText(Point(-5, -12), "Left");
  drawText(Point(25, -12), leftStr);
  drawText(Point(-5, -24), "Right");
  drawText(Point(25, -24), rightStr);
  drawText(Point(-5, -36), "Fire");
  drawText(Point(25, -36), spaceStr);
  drawPolygon(Point(-10, 26 - 12 * selection), 3, 3, selRot);
}

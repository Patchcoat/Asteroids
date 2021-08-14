/*************************************************************
 * File: pause.cpp
 * Author: Spencer Welch
 *
 * Description: Contains the implementations of the
 *  method bodies for the pause class.
 *************************************************************/

#include "pause.h"
#include "uiDraw.h"
#include "uiInteract.h"

#include <string>

Pause::Pause()
{
  selection = 0;
  maxSelect = 4;
  selRot = 0;
  visible = false;
  keyBinding = false;
  soundOptions = false;
  soundOption = SoundOptions();
}

void Pause::setVisible(bool visible)
{
  this->visible = visible;
  keyBindWindow(false);
  soundWindow(false);
  if (!visible)
  {
    selection = 0;
  }
}

void Pause::setUI(const Interface & ui)
{
  this->ui = ui;
}

void Pause::moveUp()
{
  if (keyBinding)
  {
    keyBind.moveUp();
    return;
  }
  else if (soundOptions)
  {
    soundOption.moveUp();
    return;
  }
  selection--;
  if (selection < 0)
    selection = maxSelect;
}

void Pause::moveDown()
{
  if (keyBinding)
  {
    keyBind.moveDown();
    return;
  }
  else if (soundOptions)
  {
    soundOption.moveDown();
    return;
  }
  selection++;
  if (selection > maxSelect)
    selection = 0;
}

void Pause::moveLeft()
{
  if (soundOptions)
  {
    soundOption.moveLeft();
    return;
  }
}

void Pause::moveRight()
{
  if (soundOptions)
  {
    soundOption.moveRight();
    return;
  }
}

void Pause::keyBindWindow(bool open)
{
  keyBinding = open;
  keyBind.setUI(ui);
  keyBind.setUIBinds();
  if (open)
    keyBind.setVisible(true);
  else
    keyBind.setVisible(false);
}

void Pause::soundWindow(bool open)
{
  soundOptions = open;
  soundOption.setUI(ui);
  if (open)
    soundOption.setVisible(true);
  else
    soundOption.setVisible(false);
}

int Pause::select()
{
  if (keyBinding)
  {
    int sel = keyBind.select();
    if (sel == 0)
      keyBindWindow(false);
    return -1;
  }
  else if (soundOptions)
  {
    int sel = soundOption.select();
    if (sel == 0)
      soundWindow(false);
    return -1;
  }
  return selection;
}

void Pause::advance()
{
  if (keyBinding)
    keyBind.advance();
  else if (soundOptions)
    soundOption.advance();
  selRot += 10;
  if (selRot > 360)
    selRot = 0;
}

void Pause::draw()
{
  if (!visible)
    return;
  drawScrnWdthRect(Point(0, 2), 60, 0);
  drawText(Point(-5, 24), "Resume");
  drawText(Point(-5, 12), "Restart");
  drawText(Point(-5, 0), "Sound");
  drawText(Point(-5, -12), "Rebind Keys");
  drawText(Point(-5, -24), "Quit");
  drawPolygon(Point(-10, 26 - 12 * selection), 3, 3, selRot);
  if (keyBinding)
    keyBind.draw();
  else if (soundOptions)
    soundOption.draw();
}

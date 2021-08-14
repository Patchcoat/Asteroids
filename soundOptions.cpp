#include "soundOptions.h"

SoundOptions::SoundOptions()
{
  masterVolume = 100;
  effectVolume = 50;
  musicVolume = 50;
  maxVolume = 100;
  selection = 0;
  maxSelect = 3;
  selRot = 0;
  visible = false;
  masterOn = true;
  effectOn = true;
  musicOn = true;
}

void SoundOptions::setVisible(bool visible)
{
  this->visible = visible;
  if (!visible)
  {
    selection = 0;
  }
}

void SoundOptions::setUI(const Interface & ui)
{
  this->ui = ui;
}

void SoundOptions::moveUp()
{
  selection--;
  if (selection < 0)
    selection = maxSelect;
}

void SoundOptions::moveDown()
{
  selection++;
  if (selection > maxSelect)
    selection = 0;
}

void SoundOptions::moveLeft()
{
  switch(selection)
  {
  case 0:
    break;
  case 1:
    masterVolume -= 5;
    if (masterVolume < MINVOLUME)
      masterVolume = MINVOLUME;
    break;
  case 2:
    effectVolume -= 5;
    if (effectVolume < MINVOLUME)
      effectVolume = MINVOLUME;
    break;
  case 3:
    musicVolume -= 5;
    if (musicVolume < MINVOLUME)
      musicVolume = MINVOLUME;
    break;
  }
}

void SoundOptions::moveRight()
{
  switch(selection)
  {
  case 0:
    break;
  case 1:
    masterVolume += 5;
    if (masterVolume > MAXVOLUME)
      masterVolume = MAXVOLUME;
    break;
  case 2:
    effectVolume += 5;
    if (effectVolume > MAXVOLUME)
      effectVolume = MAXVOLUME;
    break;
  case 3:
    musicVolume += 5;
    if (musicVolume > MAXVOLUME)
      musicVolume = MAXVOLUME;
    break;
  }
}

int SoundOptions::select()
{
  if (selection == 0)
    return 0;
  else
    return 1;
}

void SoundOptions::advance()
{
  selRot += 10;
  if (selRot > 360)
    selRot = 0;
}

void SoundOptions::draw()
{
  if (!visible)
    return;
  drawScrnWdthRect(Point(0, 2), 60, 0);
  drawText(Point(-5, 24), "Back");
  drawText(Point(-5, 12), "Master");
  drawText(Point(-5, 0), "Effects");
  drawText(Point(-5, -12), "Music");
  drawLine(Point(30, 14), Point(80, 14), 1, 1, 1);
  drawRect(Point(masterVolume/2 + 30, 14), 4, 6, 0);
  drawLine(Point(30, 2), Point(80, 2), 1, 1, 1);
  drawRect(Point(effectVolume/2 + 30, 2), 4, 6, 0);
  drawLine(Point(30, -10), Point(80, -10), 1, 1, 1);
  drawRect(Point(musicVolume/2 + 30, -10), 4, 6, 0);
  drawPolygon(Point(-10, 26 - 12 * selection), 3, 3, selRot);
}

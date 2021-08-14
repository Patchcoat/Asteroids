#ifndef SOUNDOPTIONS_H
#define SOUNDOPTIONS_H

#include "uiInteract.h"
#include "uiDraw.h"

#define MAXVOLUME 100
#define MINVOLUME 0

class SoundOptions
{
 private:
  Interface ui;
  float masterVolume;
  float effectVolume;
  float musicVolume;
  float maxVolume;
  int selection;
  int maxSelect;
  int selRot;
  bool visible;
  bool masterOn;
  bool effectOn;
  bool musicOn;
 public:
  SoundOptions();
  float getMasterVolume() { return masterVolume/100; };
  float getEffectVolume() { return effectVolume/100; };
  float getMusicVolume() { return musicVolume/100; };
  void setVisible(bool visible);
  void setUI(const Interface & ui);
  void moveUp();
  void moveDown();
  void moveLeft();
  void moveRight();
  int select();
  void advance();
  void draw();
};

#endif

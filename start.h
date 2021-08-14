#ifndef START_H
#define START_H

#include "uiDraw.h"
#include "point.h"

#define SHOW_TIME 206

class Start
{
 private:
  bool onStart;
  bool textVisible;
  int startMusic;
  int time;
  int selection;
  int maxSelect;
  int selRot;
 public:
  Start();
  bool getOnStart() { return onStart; };
  int getStartMusic() { return startMusic; };
  void setStartMusic(int channel) { startMusic = channel; };
  void setOnStart(bool start) { onStart = start; };
  void setTime(int time) { this->time = time; };
  void moveUp();
  void moveDown();
  int select();
  void advance();
  void draw();
};

#endif

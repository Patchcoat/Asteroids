#include "start.h"

Start::Start()
{
  onStart = true;
  textVisible = false;
  time = 0;
  selection = 0;
  maxSelect = 1;
  selRot = 0;
}

void Start::moveUp()
{
  selection--;
  if (selection < 0)
    selection = maxSelect;
}

void Start::moveDown()
{
  selection++;
  if (selection > maxSelect)
    selection = 0;
}

int Start::select()
{
  return selection;
}

void Start::advance()
{
  if (selection > maxSelect)
    selection = 0;
  selRot += 10;
  if (selRot > 360)
    selRot = 0;
  if (!textVisible)
    time++;
}

void line(Point point1, Point point2)
{
  point1.setX(point1.getX() - 200);
  point1.setY(point1.getY() - 200);
  point2.setX(point2.getX() - 200);
  point2.setY(point2.getY() - 200);
  drawLine(point1, point2, 1, 1, 1);
}

void asteroids()
{
  // TOP
  line(Point(72, 295), Point(305, 295));
  // BOTTOM
  line(Point(160, 200), Point(305, 200));
  // A
  line(Point(47, 200), Point(72, 295));
  line(Point(93, 200), Point(93, 295));
  line(Point(47, 200), Point(63, 200));
  line(Point(63, 200), Point(79, 257));
  line(Point(79, 257), Point(79, 200));
  line(Point(79, 200), Point(120, 200));
  // S
  line(Point(93, 215), Point(108, 215));
  line(Point(120, 200), Point(120, 215));
  line(Point(108, 215), Point(93, 267));
  line(Point(120, 215), Point(100, 280));
  line(Point(120, 280), Point(120, 295));
  line(Point(100, 280), Point(133, 280));
  // T
  line(Point(160, 200), Point(160, 295));
  line(Point(147, 280), Point(160, 280));
  line(Point(133, 200), Point(133, 280));
  line(Point(147, 200), Point(147, 280));
  line(Point(133, 200), Point(147, 200));
  // E
  line(Point(192, 200), Point(192, 295));
  line(Point(171, 280), Point(171, 259));
  line(Point(171, 215), Point(171, 241));
  line(Point(171, 280), Point(192, 280));
  line(Point(171, 259), Point(192, 259));
  line(Point(171, 241), Point(192, 241));
  line(Point(171, 215), Point(192, 215));
  // R
  line(Point(223, 200), Point(223, 295));
  line(Point(223, 200), Point(205, 259));
  line(Point(205, 259), Point(223, 259));
  line(Point(207, 268), Point(207, 283));
  // O
  line(Point(253, 200), Point(253, 295));
  line(Point(238, 217), Point(238, 274));
  // I
  line(Point(288, 200), Point(288, 295));
  line(Point(288, 280), Point(277, 280));
  line(Point(253, 280), Point(264, 280));
  line(Point(288, 215), Point(277, 215));
  line(Point(253, 215), Point(264, 215));
  line(Point(277, 215), Point(277, 280));
  line(Point(264, 215), Point(264, 280));
  // D
  line(Point(305, 200), Point(319, 210));
  line(Point(305, 295), Point(319, 285));
  line(Point(302, 217), Point(302, 274));
  // S
  line(Point(319, 200), Point(319, 295));
  line(Point(319, 200), Point(346, 200));
  line(Point(319, 215), Point(334, 215));
  line(Point(346, 200), Point(346, 215));
  line(Point(334, 215), Point(319, 267));
  line(Point(346, 215), Point(326, 280));
  line(Point(346, 280), Point(346, 295));
  line(Point(326, 280), Point(346, 280));
  line(Point(319, 295), Point(346, 295));
}

void Start::draw()
{
  // only show the asteroids text once it's showtime
  // in the mean time blink "stars" over the screen
  if (time < SHOW_TIME)
  {
    if (time % 6 == 0)
      for (int i = 0; i < 10; i++)
        drawDot(Point(random(-180, 180), random(-180, 180)));
    return;
  }
  textVisible = true;
  asteroids();
  drawText(Point(-5, -100), "Start");
  drawText(Point(-5, -112), "Quit");
  drawPolygon(Point(-10, -98 - 12 * selection), 3, 3, selRot);
}

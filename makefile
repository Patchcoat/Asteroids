###############################################################
# Program:
#     Milestone 10, Asteroids
#     Brother Alvey, CS165
# Author:
#     Spencer Welch
# Summary:
#     A clone of the game Astroids
# Above and Beyond
#     Added sound through fmod
#     - Music
#     - Shooting sound
#     - Sound for bullets hitting asteroids
#     - Sound for rocket hitting asteroids
#     - Rocket engine sound when accelerating
#     - Menu navagation
#     When the game is paused the music is put through a low
#      pass filter to give it a muffled sound
#     Ported the Pause menu and key rebinding menus from
#      previous projects
#     Added graphics and collision wrap around in addition to
#      the physical wrap around
#     Added a start screen
###############################################################

LFLAGS = -I./include -L./lib -lfmod -lfmodstudio -lglut -lGLU -lGL -s WASM=1 -o ast.html

###############################################################
# Build the main game
###############################################################
a.html: driver.o game.o uiInteract.o uiDraw.o soundOptions.o start.o audioEngine.o point.o velocity.o flyingObject.o ship.o bullet.o rocks.o pause.o keyBind.o asteroid.o
	emcc driver.o game.o uiInteract.o uiDraw.o soundOptions.o start.o audioEngine.o point.o velocity.o flyingObject.o ship.o bullet.o rocks.o pause.o keyBind.o asteroid.o $(LFLAGS)

###############################################################
# Individual files
#    uiDraw.o       Draw polygons on the screen and do all OpenGL graphics
#    uiInteract.o   Handles input events
#    point.o        The position on the screen
#    game.o         Handles the game interaction
#    velocity.o     Velocity (speed and direction)
#    flyingObject.o Base class for all flying objects
#    ship.o         The player's ship
#    bullet.o       The bullets fired from the ship
#    rocks.o        Contains all of the Rock classes
###############################################################
uiDraw.o: uiDraw.cpp uiDraw.h
	emcc -c uiDraw.cpp

uiInteract.o: uiInteract.cpp uiInteract.h
	emcc -c uiInteract.cpp

audioEngine.o: audioEngine.cpp audioEngine.h
	emcc -c audioEngine.cpp

soundOptions.o: soundOptions.cpp soundOptions.h
	emcc -c soundOptions.cpp

start.o: start.cpp start.h
	emcc -c start.cpp

point.o: point.cpp point.h
	emcc -c point.cpp

driver.o: driver.cpp game.h
	emcc -c driver.cpp

game.o: game.cpp game.h uiDraw.h uiInteract.h point.h velocity.h flyingObject.h bullet.h rocks.h ship.h pause.h
	emcc -c game.cpp

velocity.o: velocity.cpp velocity.h point.h
	emcc -c velocity.cpp

flyingObject.o: flyingObject.cpp flyingObject.h point.h velocity.h uiDraw.h
	emcc -c flyingObject.cpp

ship.o: ship.cpp ship.h flyingObject.h point.h velocity.h uiDraw.h
	emcc -c ship.cpp

bullet.o: bullet.cpp bullet.h flyingObject.h point.h velocity.h uiDraw.h
	emcc -c bullet.cpp

rocks.o: rocks.cpp rocks.h flyingObject.h point.h velocity.h uiDraw.h
	emcc -c rocks.cpp

pause.o: pause.cpp pause.h keyBind.h soundOptions.h
	emcc -c pause.cpp

keyBind.o: keyBind.cpp keyBind.h
	emcc -c keyBind.cpp

asteroid.o: asteroid.cpp asteroid.h
	emcc -c asteroid.cpp

###############################################################
# General rules
###############################################################
clean:
	rm a.out *.o

#ifndef MAIN_H
#define MAIN_H

#include <SDL.h>

#include "game.h"

extern Game game;

extern SDL_Renderer *renderer;
extern float renderScale;
extern int screenleft,screentop;
extern int screenw,screenh;
extern int maptop;

#include <string>
#include <sstream>

#endif

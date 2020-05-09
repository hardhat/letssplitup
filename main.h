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
//#if 0
#if (_cplusplus < 201103L)
#include <sstream>
namespace std {
	string to_string(int a);
};

#endif

#endif

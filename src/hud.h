#ifndef HUD_H
#define HUD_H
#include <deque>

#include <SDL/SDL.h>

class Falling;

class Sprite;
typedef std::deque<Sprite *> SpriteList;

class Hud
{
public:
	Hud();
	void update(SpriteList &list);
	void draw();
	void drawDigit(int x,int y,int i);
	void drawDigit(SDL_Surface *icon,int x,int y,int i);
	void drawArrow(int x,int y,int i);
	void drawIcon(int x,int y, SDL_Surface *surf);
	void animateScore(int viewx,int viewy,Falling *who);

	SDL_Surface *numbers,*victory[4],*icon[4];
	int score[4];
	Falling *who;
	float whoX,whoY;
	float heroX[4],heroY[4];
	bool leftActive,rightActive,jumpActive;
};

#endif

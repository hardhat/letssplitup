#ifndef MAP_H
#define MAP_H

#ifdef _PSP
#include <oslib/oslib.h>
#else
#include <SDL/SDL.h>
#ifdef __APPLE__
#include <SDL_image/SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif
#endif

#include <string>

class Falling;
class TileCollide;

class Map
{
protected:
	std::string level;
#ifdef _PSP
	OSL_IMAGE *backgroundImage;
	OSL_IMAGE *levelImage;
	OSL_IMAGE *collisionImage;
#else
	SDL_Surface *backgroundImage;
	SDL_Surface *levelImage;
	SDL_Surface *collisionImage;
#endif
	int cellw,cellh;
	int tilesAcross,tilesDown;
	int *cell;
	int *gradient;
	void drawTile(int tile,int x,int y);
	TileCollide *getCollide(int tile);
    bool gradientCollide(int tile);
    int gradientValue(float newx,float newy);
    void setGradient(int cx,int cy,int id);
public:
	int viewx,viewy;
	int viewTimer;
	Map();
	~Map();
	void load(const char *level);
	bool collide( Falling *object, float newx, float newy);
	void draw();
	void updatePhysics();
	void updateView(Falling *hero);
	void updateView(Falling *hero,Falling *baddie);
	void calculateGradient(Falling *target);
	void getDirection(Falling *baddie,int &ax,int &ay);
	int getTilesAcross() { return tilesAcross; }
	int getTilesDown() { return tilesDown; }
};

#endif

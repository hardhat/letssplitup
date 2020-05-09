#ifndef SPRITE_H
#define SPRITE_H

#include<deque>

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

#include "falling.h"

#define FACING_LEFT -1
#define FACING_RIGHT 1
#define XACCEL (0.2f)

enum CharacterState {
	CS_STANDING,
	CS_RUNNING,
	CS_JUMPING,
	CS_FALLING,
	CS_LANDING,
	CS_DEAD,
	CS_PUNCHING,
	CS_HIT
	};

class Map;

class Sprite : public Falling
{
public:
	float vx;
	float ax;
private:
#ifdef _PSP
	OSL_IMAGE *image;
#else
	SDL_Surface *image;
#endif
	int cellw;	// in pixels
	int cellh;	// in pixels
	int frame;	// currently displayed frame in an animation loop
	int frameTimer;	// how long until the frame changes
	int state;
	int facing;	// which way the sprite is facing 1=right, -1=left
public:
	Sprite(const char *spriteSheet,int cellWidth,int cellHeight);
	~Sprite();
	/// Show the sprite.
	void draw(int viewx,int viewy);
	/// Called every 1/60th of a second.
	void updatePhysics(Map *map);
	/// The left arrow was pressed or released.
	void moveLeft(bool down);
	/// The right arrow was pressed or released.
	void moveRight(bool down);
	/// The jump button was pressed or released.
	void jump(bool down);
	void reset(int x,int y);
	void ai(Map *map, Sprite *target);
};

typedef std::deque<Sprite *> SpriteList;

#endif

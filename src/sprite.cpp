#include <stdio.h>
#include "sprite.h"
#include "map.h"
#include "main.h"
#ifdef __APPLE__
#include <SDL_image/SDL_image.h>
#include <SDL_mixer/SDL_mixer.h>
#else
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#endif

#ifndef _PSP
extern SDL_Surface *screen;
#else
extern struct Screen {
	int w,h;
} *screen;
#endif

// OSL_Image *image;
// int cellw;
// int cellh;
// int frame,frameTimer;
// int direction;

Sprite::Sprite(const char *spriteSheet,int cellWidth,int cellHeight)
{
	cellw=cellWidth;
	cellh=cellHeight;
	width=cellWidth*3/4;
	height=cellHeight;
#ifdef _PSP
	image=oslLoadImageFile((char *)spriteSheet, OSL_IN_RAM, OSL_PF_5551);
#else
	image=IMG_Load(spriteSheet);
#endif
	frame=0;
	facing=FACING_RIGHT;
	ax=0;
	vx=0;
	state=CS_STANDING;
	frameTimer=1;
}

Sprite::~Sprite()
{
#ifdef _PSP
	if(image) oslDeleteImage(image);
#else
	if(image) SDL_FreeSurface(image);
#endif
	image=0;
}

void Sprite::draw(int viewx,int viewy)
{
	int top,left;
	left=(frame%10)*cellw; // convert from tiles across to pixels across.
	top=(frame/10)*cellh;	// convert from tiles down to pixels down.
#ifdef _PSP
	oslSetImageTileSize(image,left,top,cellw,cellh);	// set the sprite sheet up to show that sprite.

	image->x=x-width/2-viewx;	// draw it where it goes
	image->y=y-height/2-viewy+4;	// draw it where it goes
	if(facing==FACING_LEFT) oslMirrorImageH(image);	// flip
	oslDrawImage(image);
	if(facing==FACING_LEFT) oslMirrorImageH(image);	// return to normal.
#else
	// doesn't do facing. :(
	SDL_Rect src={static_cast<Sint16>(left),static_cast<Sint16>(top),static_cast<Uint16>(cellw),static_cast<Uint16>(cellh)},dest={static_cast<Sint16>(x-width/2-viewx),static_cast<Sint16>(y-height/4-viewy),static_cast<Uint16>(cellw),static_cast<Uint16>(cellh)};
	SDL_BlitSurface(image,&src,screen,&dest);
#endif
}

void Sprite::moveLeft(bool down)
{
	ax=down?-XACCEL:0;
	facing=FACING_LEFT;
}

void Sprite::moveRight(bool down)
{
	ax=down?XACCEL:0;
	facing=FACING_RIGHT;
}

void Sprite::jump(bool down)
{
	ay=down?JUMP:0;
	printf("Jump ay=%.2f\n",ay);
	if(down) playSound(S_JUMP);
}

void Sprite::updatePhysics(Map *map)
{
	updateGravity(map);
	vx=vx+ax;
	vx=vx*0.95f;	// air friction.
	if(vx>0.001f && vx<0.001f) vx=0;  // avoid underflow.
	float newx=x+vx;
	int i;
	for(i=0;i<8;i++) {
		if( !map->collide((Falling *)this,newx,y)) break;
		vx/=2.0f; // didn't make it, so try half the distance.
		newx=x+vx;
	}
	if(i<8) x=newx;
	frameTimer-=1;
	if(frameTimer<0) {
		frameTimer=3;
        if(facing==FACING_LEFT){
            if(state==CS_RUNNING)	frame=20+(((frame+1)-20)%6);
            if(state==CS_STANDING) frame=30;
            if(state==CS_JUMPING) frame=26;
            if(state==CS_FALLING) frame=40;	// was 8
            if(state==CS_LANDING) frame=27;
            if(state==CS_DEAD) frame=41;	// was 9
            if(state==CS_PUNCHING) frame=11+((frame+1)%3);
            if(state==CS_HIT) frame=15;
        }// flip
        if(facing==FACING_RIGHT){
            if(state==CS_RUNNING)	frame=(frame+1)%6;
            if(state==CS_STANDING) frame=10;
            if(state==CS_JUMPING) frame=6;
            if(state==CS_FALLING) frame=20;	// was 8
            if(state==CS_LANDING) frame=7;
            if(state==CS_DEAD) frame=21;	// was 9
            if(state==CS_PUNCHING) frame=11+((frame+1)%3);
            if(state==CS_HIT) frame=15;

        }// return to normal.
	}

	if(vx<-1.0f || vx>1.0f) state=CS_RUNNING;
	else state=CS_STANDING;
	if(vy>0.1f) state=CS_JUMPING;
	if(vy<-0.1f) state=CS_LANDING;
}

void Sprite::ai(Map *map,Sprite *target)
{
	/* Plan path. */
    int dirx=0,diry=0;

    map->getDirection(this,dirx,diry);

	if(diry>0) {
		ay=JUMP;
	} else {
		ay=0;
	}
	if(dirx<0) {
		facing=FACING_LEFT;
		ax=-XACCEL*3/5;
	} else if(dirx>0) {
		facing=FACING_RIGHT;
		ax=XACCEL*3/5;
	} else {
		ax=0;
	}
}

void Sprite::reset(int x,int y)
{
	this->x=x;
	this->y=y;
	vx=0;
	vy=0;
	ax=0;
	ay=0;
	
}

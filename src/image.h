//
//  image.h
//  ninjashoppingmadness
//
//  Created by Daniel Sutherland on 2015-05-03.
//  Copyright (c) 2015 Team Sushi. All rights reserved.
//

#ifndef __ninjashoppingmadness__image__
#define __ninjashoppingmadness__image__

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
//
//enum CharacterState {
//    CS_STANDING,
//    CS_RUNNING,
//    CS_JUMPING,
//    CS_FALLING,
//    CS_LANDING,
//    CS_DEAD,
//    CS_PUNCHING,
//    CS_HIT
//};

class Map;

class Image : public Falling
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
    Image(const char *imageFile);
    ~Image();
    /// Show the sprite.
    void draw(int viewx,int viewy);
    void updatePhysics(Map *map);
    void reset(int x,int y);
};

#endif

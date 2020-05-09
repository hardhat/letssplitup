#ifndef FALLING_H
#define FALLING_H

class Map;

#define GRAVITY (9.8f/30.0f)
#define MAXVEL 15.0f
#define MINVEL -9.0f
#define JUMP (-GRAVITY*2)

class Falling {
public:
	Falling();
	int score;
	float x,y; // centre
	float width,height;  // rectangle is (x-width/2,y-height/2) to (x+width/2, y+height/2)
	float vy; // velocity vertically
	float ay; // acceleration vertically
	void updateGravity(Map *map); // called every 1/60th of a second.
};

#endif

#include <stdio.h>
#include "falling.h"
#include "map.h"
#include "hud.h"

Falling::Falling()
{
	vy=0;
	ay=0;
	x=0;
	y=0;
	width=1;
	height=1;
	score=0;
}

void Falling::updateGravity(Map *map)
{
	if( ay!=0) {
		printf("y=%.2f; vy=%.2f; ay=%.2f\n",y,vy,ay);
	}
	vy=vy+ay;
	vy=vy+GRAVITY;   // earth gravity is 9.8 m/s^2, or whatever it works out to in your units.
	// apply terminal velocity
	if( vy<MINVEL) {
		vy=MINVEL;
		ay=0;  // hit minimum negative velocity so stop jumping.
	}
	if( vy>MAXVEL) {
		vy=MAXVEL;
		ay=0;  // hit maximum velocity so stop jumping.
	}
	// propose new position, and test for collision:
	float newy=y+vy;
	int i;
	for( i=0; i<8; i++) {
		if( !map->collide( this, x, newy) ) break; // a safe place to move the item to.
		// we don't want to be inside the object, so guess we can move half the distance.
		vy/=2.0f;
		newy=y+vy;
	}
	if( i==8) {
		newy=y; // we can't find a place closer to the ground/ceiling so we need to stop moving.
		vy=0;
		ay=0;
	}
	y=newy; // our new vertical position with gravity and jumping factored in!
}


#include "bullet.h"

Bullet::Bullet(AttackType type,float startx,float starty,float targetx,float targety,int timeToLive,Actor *attackTarget,int hp) :
	Sprite(type==AT_FIRE?"fireball.png":"force.png")
{
	x=startx;
	y=starty;
	vx=(targetx-startx)/(float)timeToLive;
	vy=(targety-starty)/(float)timeToLive;
	timer=timeToLive;
    attackType=type;
	target=attackTarget;
	damage=hp;
}

void Bullet::update(int elapsed)
{
	if(timer>0) {
		x+=vx*elapsed;
		y+=vy*elapsed;
		if(timer>elapsed) timer-=elapsed; else timer=0;
	}
}

bool Bullet::isActive()
{
	return timer>0;
}

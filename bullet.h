#ifndef BULLET_H
#define BULLET_H

#include "sprite.h"

enum AttackType {
	AT_NONE,
	AT_FORCE,
	AT_FIRE,
	AT_ICE,
	AT_LIGHTNING,
	AT_HEALING,
};

class Actor;

class Bullet : public Sprite {
public:
	Bullet(AttackType type,float sourcex,float sourcey,float targetx,float targety,int timeToLive,Actor *attackTarget,int hp);
	float vx,vy;	// speed per 1 ms.
	int timer;	// time to live.
	AttackType attackType;
	Actor *target;
   	int damage;
	
	void update(int elapsed);
	bool isActive();
};

#endif

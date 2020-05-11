#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

class Tile;

class Player : public Actor {
public:
    int type;

	Player(Tile *tile, int x, int y);
	virtual void resetGame();
	virtual void resetGame(int type,int tx,int ty);
	virtual void update(int elapsed);
	virtual void draw();
	virtual void handleAction(int id,bool down);

	virtual void setType(int type);
	int getAvatarId(int type);
	int avatarId;
	int getAvatarDeadId(int type);
	bool right, left, up, down;
	bool jump = false;
	int speedx = 0, speedy = 1;
	int animationDelay = 10;
	float ay, vy;
    double gravity = (9.8f/30.0f);
    double MAXVEL = 15.0f;
    double MINVEL = -9.0f;
    double JUMP = (-(9.8f/30.0f)*2);
    bool landed = true;
};

#endif

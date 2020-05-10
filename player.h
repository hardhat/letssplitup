#ifndef PLAYER_H
#define PLAYER_H
#include "actor.h"

class Tile;

class Player : public Actor {
public:
    int type;

	Player(Tile *tile);
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
	int speedx = 0, speedy = 0;
	int animationDelay = 10;
};

#endif

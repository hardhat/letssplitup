#ifndef ACTOR_H
#define ACTOR_H

#include <deque>


#include "bullet.h"
#include "notice.h"

typedef std::deque<Bullet *> BulletList;
typedef std::deque<Notice *> NoticeList;

class Tile;

class Actor {
public:
	int health;
	int fullHealth;
	AttackType attackType;
	int attackTimer;
	int attackRegenerateTime;
	int mode;
    bool spoke;

	int resistance[6];	// percent of how resistant they are to various attacks

	AttackType blockType;
	int blockTimer;
	int blockRegenerateTime;
	bool enemy;

	int avatarId;
	int avatarDeadId;
	int shieldId;
	int avatarWidth;    // in tiles
	int avatarHeight;   // in tiles
	int tx,ty;

	BulletList bulletList;
	NoticeList noticeList;

	Tile *tile;

	Actor(Tile *tile);
	virtual ~Actor();
	virtual void resetGame();
	virtual void resetGame(int tx,int ty);
	virtual void update(int elapse);
	virtual void draw();
	virtual void handleAction(int id,bool down);

	bool isAlive();
	bool isAttackReady();
	void attack(Actor *target,bool heavy);
	void receiveAttack(int amount, AttackType type);
	bool attackInRange(Bullet *b);
	void block(AttackType type);
	void enemyAttack();
};

#endif

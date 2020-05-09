#include <stdio.h>

#include "main.h"
#include "actor.h"
#include "sound.h"
#include "player.h"

Actor::Actor(Tile *tile)
{
	//printf("Actor::Actor()\n");
    this->tile=tile;

	attackType=AT_FORCE;
	fullHealth=100;
	attackRegenerateTime=1000;

	for(int i=0;i<6;i++) {
		resistance[i]=0;	// percent of how resistent they are to various attacks
	}

	blockType=AT_NONE;
	blockRegenerateTime=1000;
	enemy=false;

    avatarId=3;
    avatarDeadId=2;
    shieldId=1;
    tx=1;
    ty=1;
    avatarWidth=1;
    avatarHeight=1;
    spoke=false;
}

void Actor::resetGame()
{
    avatarId=54;
    avatarDeadId=53;
    shieldId=4;
    tx=5;
    ty=1;
    avatarWidth=1;
    avatarHeight=1;

	blockTimer=0;
	attackTimer=0;
	health=fullHealth;
	spoke=false;

	for(BulletList::iterator p=bulletList.begin();p!=bulletList.end();p++) {
		Bullet *b=*p;
		delete b;
	}
	bulletList.clear();

	for(NoticeList::iterator p=noticeList.begin();p!=noticeList.end();p++) {
		Notice *n=*p;
		delete n;
	}
	noticeList.clear();

}

void Actor::resetGame(int x,int y)
{
    resetGame();
    tx=x;
    ty=y;
}

Actor::~Actor()
{
}

void Actor::update(int elapsed)
{
	if(attackTimer>=elapsed) attackTimer-=elapsed; else attackTimer=0;
	if(blockTimer>=elapsed) blockTimer-=elapsed; else blockTimer=0;

	if(enemy) {
        enemyAttack();
        if(!spoke && bulletList.size()>0) {
            spoke=true;
            if(avatarId==76) sound.playOnce(SFX_WELCOME);
        }
	}
	if(avatarId==56 && !spoke && maptop<100*tile->tileHeight) {
        spoke=true;
        sound.playOnce(SFX_QUEST);
	}

	bool deadBullet=false;
	for(BulletList::iterator p=bulletList.begin();p!=bulletList.end();p++) {
		Bullet *b=*p;
		b->update(elapsed);
		if(!b->isActive()) deadBullet=true;
	}
	if(deadBullet) {
		BulletList list=bulletList;
		bulletList.clear();
		for(BulletList::iterator p=list.begin();p!=list.end();p++) {
			Bullet *b=*p;
			if(b->isActive()) {
				bulletList.push_back(b);
			} else {
			    if(b->target->attackInRange(b)) {
                    b->target->receiveAttack(b->damage,b->attackType);
			    }
				delete b;	// all done.
			}
		}
	}

	bool deadNotice=false;
	for(NoticeList::iterator p=noticeList.begin();p!=noticeList.end();p++) {
		Notice *n=*p;
		n->update(elapsed);
		if(!n->isActive()) deadNotice=true;
	}
	if(deadNotice) {
		NoticeList list=noticeList;
		noticeList.clear();
		for(NoticeList::iterator p=list.begin();p!=list.end();p++) {
			Notice *n=*p;
			if(n->isActive()) {
				noticeList.push_back(n);
			} else {
				delete n;	// all done.
			}
		}
	}
}

void Actor::draw()
{
	//printf("%s: %d HP, %d ATK, %d BLK\n",enemy?"Dragon":"Player",health,attackTimer,blockTimer);
	//printf("blockRegenerateTime=%d, attackRegenerateTime=%d\n",blockRegenerateTime, attackRegenerateTime);

	if(health>0) {
		for(int j=0;j<avatarHeight;j++) {
            for(int i=0;i<avatarWidth;i++) {
                tile->draw(avatarId+i+j*10,tx+i,ty+j);
            }
		}
	} else if(health==0) {
		tile->draw(avatarDeadId,tx,ty);
	}

    if(blockTimer>0) {
        tile->draw(shieldId,tx,ty);
	}

	for(BulletList::iterator p=bulletList.begin();p!=bulletList.end();p++) {
		Bullet *b=*p;
		b->draw();
	}
	for(NoticeList::iterator p=noticeList.begin();p!=noticeList.end();p++) {
		Notice *n=*p;
		n->draw();
	}

	if(avatarId==76 && health>0) {
        int x=(tx-2)*tile->tileWidth;
        int y=ty*tile->tileHeight-5;
        static int color=0;
        Uint8 r=32, g=255, b=32;
        color=(color+1)%32;
        SDL_SetRenderDrawColor(renderer,255,32,32,255);
        SDL_Rect rect={(int)((x-8)*renderScale)+screenleft,(int)((y-8-maptop)*renderScale)+screentop,(int)(tile->tileWidth*6*renderScale),(int)(5*renderScale)};
        SDL_RenderFillRect(renderer,&rect);
        SDL_SetRenderDrawColor(renderer,r,g,b,255);
        float percent=health/(float)fullHealth;
        SDL_Rect rect2={(int)((x-8)*renderScale)+screenleft,(int)((y-8-maptop)*renderScale)+screentop,(int)(percent*tile->tileWidth*6*renderScale),(int)(5*renderScale)};
        SDL_RenderFillRect(renderer,&rect2);
	}
}

bool Actor::isAlive() {
	return health>0;
}

bool Actor::isAttackReady() {
	if(health==0) return false;
	if(!enemy && attackTimer<=0) return true;
	return attackTimer<=0;
}

void Actor::attack(Actor *target, bool heavy) {
	if(health==0) return;
	if(attackTimer==0 && blockTimer==0) {
		//printf("%s: is attacking with a %s attack of type %d\n",enemy?"Dragon":"Player",heavy?"heavy":"light",(int)attackType);

		//target->receiveAttack(heavy?10:5,attackType);
        //printf("Attack active for %d ms.\n",attackTimer);
		float sx=0,sy=0,tx=0,ty=0;
        sx=this->tx*tile->tileWidth;
        sy=this->ty*tile->tileHeight;
        tx=target->tx*tile->tileHeight;
        ty=target->ty*tile->tileWidth;

		float dx,dy;
		dx=sx-tx;
		dx=dx<0?-dx:dx;
		dy=sy-ty;
		dy=dy<0?-dy:dy;

		//attackTimer=attackRegenerateTime*(heavy?2:1);
		attackTimer=attackRegenerateTime*(dx+dy)/(100);
		bulletList.push_back(new Bullet(attackType,sx+16,sy,tx,ty,attackRegenerateTime*(heavy?2:1),target,heavy?10:5));
	}
}

void Actor::receiveAttack(int amount, AttackType type) {
	float sx=0,sy=0;
    sx=this->tx*tile->tileWidth;
    sy=this->ty*tile->tileHeight;

	if(health==0) return;	// already dead.

	if(blockTimer==0) {
		health-=amount;
		if(health<0) health=0;
		//printf("%s: Received attack of %d and type %d.  %d HP remaining.\n",enemy?"Dragon":"Player",amount,(int)type,health);
		if(health==0) {
			if(!enemy) sound.playOnce(SFX_DIE);
			noticeList.push_back(new Notice(sx,sy,"Dead"));
		} else {
			static int ugh=0;
			std::string message=std::to_string(-amount);
			noticeList.push_back(new Notice(sx,sy,message.c_str()));
			if(enemy) {
				sound.playOnce(SFX_ZAP);
			} else {
				sound.playOnce(ugh!=0?ugh==2?SFX_UGH1:SFX_UGH2:SFX_UGH3);
				ugh=(ugh+1)%3;
			}
		}
	} else {
		//printf("%s: Blocked attack.\n",enemy?"Dragon":"Player");
		sound.playOnce(SFX_HAH);
		noticeList.push_back(new Notice(sx,sy,"Blocked"));
	}
}

bool Actor::attackInRange(Bullet *b)
{
    float dx=b->x-(this->tx*tile->tileWidth);
    float dy=b->y-(this->ty*tile->tileHeight);

    return dx*dx+dy*dy<24*24;
}

void Actor::block(AttackType type) {
	if(health==0) return;

	//printf("Block requested.\n");
	if(blockTimer==0 && attackTimer==0) {
		blockType=type;
		blockTimer=blockRegenerateTime;
		//printf("Block active for %d ms.\n",blockTimer);
	}
}

void Actor::handleAction(int id,bool down)
{
}

void Actor::enemyAttack()
{
    if(!enemy) return;
    if(isAttackReady()) {
        // if it is time attack, then we attack.
        if(avatarId==76) {
            for(PlayerList::iterator p=game.playerList.begin();p!=game.playerList.end();p++) {
                Actor *player=(Actor *)*p;

                if(player->ty>16) continue;
                if(player->health<=0) continue;

                attackTimer=0;
                attack((Actor *)player,false);
            }
            return;
        }

        Actor *player=(Actor *)game.targetPlayer(this);
        if(!player) return;

        attack(player,mode<6);
        mode++;
        if(mode>=12) mode=0;
    }
}

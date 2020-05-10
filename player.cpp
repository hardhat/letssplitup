#include <stdio.h>

#include "main.h"
#include "sound.h"
#include "player.h"

Player::Player(Tile *tile) : Actor(tile)
{
	printf("Player::Player()\n");
	resetGame();
	type=0;
}

void Player::resetGame()
{
	//Actor::resetGame();
    setType(0);
    tx=1;
    ty=7;
}

void Player::resetGame(int type,int x,int y)
{
    resetGame();
    setType(type);
    tx=x;
    ty=y;
}

void Player::update(int elapsed)
{
	Actor::update(elapsed);
	int x=tx,y=ty;

    if(speedx != 0 || speedy != 0) {
        x += speedx;
        y += speedy;
        if(game.canMoveTo(this,x,y)) {
            tx += speedx;
            ty += speedy;
        }
    }


}

void Player::draw()
{
    //printf("tx = %d, ty = %d\n",tx,ty);
    tile->draw(0,tx,ty);
	//Actor::draw();
}

void Player::handleAction(int id,bool down)
{
    //printf("x = %d, y = %d", x, y);
    //Actor::handleAction(id,down);

    if(health<=0) return;
    else if(id==DPAD_LEFT && down && tx>0) speedx = -1, left = true;
    else if(id==DPAD_RIGHT && down && tx<19) speedx = 1, right = true;
    else if(id==DPAD_UP && down && ty>0) speedy = -1, up = true;
    else if(id==DPAD_DOWN && down && ty<119) speedy = 1, down = true;
    else if(id==DPAD_A) {
        Actor *target=game.targetEnemy(this);
        if(target) attack(target,true);
    }
    else if(id==DPAD_B) {
        //Actor *target=game.targetEnemy(this);
        //if(target) attack(target,true);
        block(AT_FORCE);
    }
    else if(id==DPAD_LEFT && !down && tx>0) speedx = 0, left = false;
    else if(id==DPAD_RIGHT && !down && tx<19) speedx = 0, right = false;
    else if(id==DPAD_UP && !down && ty>0) speedy = 0, up = false;
    else if(id==DPAD_DOWN && !down && ty<119) speedy = 0, down = false;
    //printf("newx = %d, newy = %d", x, y);
}

void Player::setType(int newType)
{
    type=newType;
    if(type<0) type=2;
    if(type>2) type=0;

    avatarId=getAvatarId(type);
    avatarDeadId=getAvatarDeadId(type);
    shieldId=4;
}

int Player::getAvatarId(int type)
{
    switch(type) {
    case 1:
        return 41;
    case 2:
        return 42;
    }
    return 40;
}

int Player::getAvatarDeadId(int type)
{
    switch(type) {
    case 1:
        return 51;
    case 2:
        return 52;
    }
    return 50;
}

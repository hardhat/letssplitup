#include <stdio.h>

#include "main.h"
#include "sound.h"
#include "player.h"

Player::Player(Tile *tile, int x, int y) : Actor(tile)
{
	printf("Player::Player()\n");
	avatarId = 0;
	tx = x;
	ty = y;
	vy = 0;
	ay = 0;
	jump = false;
	resetGame();
	landed = true;
	//type=0;
}

void Player::resetGame()
{
	//Actor::resetGame();
    //setType(0);
    avatarId = 0;
    tx=3;
    ty=6;
    vy = 0;
	ay = 0;
	jump = false;
	landed = true;
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
	gravity = (9.8f/30.0f);
	MINVEL = -9.0f;
	MAXVEL = 15.0f;
	int x=tx,y=ty;
	vy = vy + ay;
	vy = vy + gravity;
	animationDelay--;
	while(animationDelay == 0){
        avatarId += 1;
        if(avatarId == 3){
            avatarId = 1;
        }
        animationDelay = 10;
	}
    if(vy < MINVEL){
        vy = MINVEL;
        ay = 0;
    }
    if(vy > MAXVEL){
        vy = MAXVEL;
        ay=0;
    }

    float newy = y + vy;
    int i;
    for(i = 0; i<9; i++){
        //vy/=2.0f;
        newy = y + vy;
    }
    if(i == 9 && !jump){
        newy = y;
        ay = 0;
        vy = 0;
        //speedy = 1;
    }
    y = newy;
    if(speedx != 0 || speedy != 0 || y != 0) {
        x += speedx;
        if(!jump){
            y += speedy;
        }
        if(game.canMoveTo(this,x,ty)) {
            tx += speedx;
        } else {
            speedx = 0;
        }
        if(game.canMoveTo(this, tx, y)){
            ty += speedy;
            if(jump == true){
               ty = y;
            }
        } else {
            landed = true;
            vy = 0;
        }
    }
}

void Player::draw()
{
    //printf("tx = %d, ty = %d\n",tx,ty);
    //renderScale = 2;
    tile->draw(avatarId,tx,ty);
	//Actor::draw();
}

void Player::handleAction(int id,bool down)
{
    //printf("x = %d, y = %d", x, y);
    //Actor::handleAction(id,down);
    if(health<=0) return;
    else if(id==DPAD_LEFT && down && tx>0) speedx = -1,  left = true;
    else if(id==DPAD_RIGHT && down && tx<19) speedx = 1,  right = true;
    else if(id==DPAD_SELECT && down && tx >0){
        if(!jump){
            jump = true, landed = false, ay = JUMP;
        }
    }
    //else if(id==DPAD_UP && down && ty>0) speedy = -1, up = true;
    //else if(id==DPAD_DOWN && down && ty<119) speedy = 1, down = true;
    else if(id==DPAD_A) {
        Actor *target=game.targetEnemy(this);
        if(target) attack(target,true);
    }
    else if(id==DPAD_B) {
        //Actor *target=game.targetEnemy(this);
        //if(target) attack(target,true);
        block(AT_FORCE);
    }
    //else if(down) speedy = 0;
    else if(id==DPAD_LEFT && !down && tx>0) speedx = 0,  left = false;
    else if(id==DPAD_RIGHT && !down && tx<19) speedx = 0,  right = false;
    else if(id==DPAD_SELECT && !down && tx >0) jump = false, ay = 0;
    //else if(!down) speedy = 1;

    /*if(jump){
        ay=down?JUMP:0;
    } */
    //jump = down;
    //else if(id==DPAD_UP && !down && ty>0) speedy = 0, up = false;
    //else if(id==DPAD_DOWN && !down && ty<119) speedy = 0, down = false;
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

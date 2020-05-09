#include <stdio.h>

#include "main.h"
#include "sound.h"
#include "player.h"

Player::Player(Tile *tile) : Actor(tile)
{
	//printf("Player::Player()\n");
	//resetGame();
	type=0;
}

void Player::resetGame()
{
	Actor::resetGame();
    setType(0);
    tx=10;
    ty=12;
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
}

void Player::draw()
{
	Actor::draw();

    int x=tx*tile->tileWidth;
    int y=ty*tile->tileHeight-5;
    static int color=0;
    Uint8 r=32, g=255, b=32;
    color=(color+1)%32;
    SDL_SetRenderDrawColor(renderer,255,32,32,255);
    SDL_Rect rect={(int)((x-8)*renderScale)+screenleft,(int)((y-8-maptop)*renderScale)+screentop,(int)(tile->tileWidth*renderScale),(int)(5*renderScale)};
    SDL_RenderFillRect(renderer,&rect);
    SDL_SetRenderDrawColor(renderer,r,g,b,255);
    float percent=health/(float)fullHealth;
    SDL_Rect rect2={(int)((x-8)*renderScale)+screenleft,(int)((y-8-maptop)*renderScale)+screentop,(int)(percent*tile->tileWidth*renderScale),(int)(5*renderScale)};
    SDL_RenderFillRect(renderer,&rect2);
}

void Player::handleAction(int id,bool down)
{
    int x=tx,y=ty;

    Actor::handleAction(id,down);

    if(health<=0) return;
    if(id==DPAD_LEFT && down && tx>0) x--;
    else if(id==DPAD_RIGHT && down && tx<19) x++;
    else if(id==DPAD_UP && down && ty>0) y--;
    else if(id==DPAD_DOWN && down && ty<119) y++;
    else if(id==DPAD_A) {
        Actor *target=game.targetEnemy(this);
        if(target) attack(target,true);
    }
    else if(id==DPAD_B) {
        //Actor *target=game.targetEnemy(this);
        //if(target) attack(target,true);
        block(AT_FORCE);
    }

    if(x!=tx || y!=ty) {
        if(game.canMoveTo(this,x,y)) {
            tx=x;
            ty=y;
            int id=game.collect(tx,ty);
            if(id!=-1) {
                float sx=0,sy=0;
                sx=tx*tile->tileWidth;
                sy=ty*tile->tileHeight;

           		sound.playOnce(SFX_HAH);

           		switch(id) {
                case 21:    // goblet
                    noticeList.push_back(new Notice(sx,sy,"Time -10"));
                    game.totalTime-=10000;
                    if(game.totalTime<0) game.totalTime=0;
                    break;
                case 45:    // fish
                case 23:    // candle
                    noticeList.push_back(new Notice(sx,sy,"+health"));
                    health=fullHealth;
                    break;
           		}
            }
        }
    }
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

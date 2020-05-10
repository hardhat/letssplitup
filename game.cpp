#include <stdio.h>
#include <SDL.h>
#include "main.h"
#include "game.h"
#include "tile.h"
#include "layer.h"
#include "font.h"
#include "sound.h"
#include "actor.h"
#include "player.h"

Game game;

Game::Game() {
    tile=0;
    mode=0;
    bgLayer=0;
    fgLayer=0;
    characterLayer=0;
}

Game::~Game() {
}

void Game::resetGame()
{
    for(PlayerList::iterator p=playerList.begin();p!=playerList.end();p++) {
        Player *player=*p;
        delete player;
    }
    playerList.clear();

    for(ActorList::iterator p=enemyList.begin();p!=enemyList.end();p++) {
        Actor *enemy=*p;
        delete enemy;
    }
    enemyList.clear();

    if(!tile) {
        tile=new Tile("spritesheet.png",128,128);
        bgLayer=new Layer(tile);
        fgLayer=new Layer(tile);
        characterLayer=new Layer(tile);
    }

    bgLayer->load("data/example_background.csv");
    fgLayer->load("data/example_foreground.csv");
    characterLayer->load("data/example_character.csv");


    while(playerList.size()<1) {
        Player *player=new Player(tile);
        player->resetGame();
        playerList.push_back(player);
    }

    setMode(MODE_LEVEL);
    totalTime=0;
}

void Game::update(int elapsed)
{
    if(playerList.size()==0) return;
    int cy;
    cy=0;
    int count=0;

    spawnEnemies();

    for(ActorList::iterator p=enemyList.begin();p!=enemyList.end();p++) {
        Actor *enemy=*p;
        enemy->update(elapsed);
    }

    for(PlayerList::iterator p=playerList.begin();p!=playerList.end();p++) {
        Player *player=*p;

        player->update(elapsed);
        if(!player->isAlive()) continue;
        cy+=player->ty;
        count++;
    }

    if(count>0 && !playersWin()) {
        totalTime+=elapsed;
        cy/=count;
        maptop=(cy-10)*16;
        if(maptop<0) maptop=0;
        if(bgLayer && cy-10+15>bgLayer->th) maptop=(bgLayer->th-15)*16;
    }
}

void Game::draw()
{
    int count=0;

    bgLayer->draw();
    fgLayer->draw();

    for(ActorList::iterator p=enemyList.begin();p!=enemyList.end();p++) {
        Actor *enemy=*p;
        //enemy->draw();
    }
    for(PlayerList::iterator p=playerList.begin();p!=playerList.end();p++) {
        Player *player=*p;
        player->draw();
        if(player->isAlive()) count++;
    }

    int w=0,h=0;
    char buf[256];
    sprintf(buf,"%d:%02d",totalTime/60000,(totalTime%60000)/1000);
    extentMessage(FONT_HEADLINE,buf,w,h);
    drawMessage(FONT_HEADLINE,buf,screenw-w,maptop);

    if(playersWin()) {
        w=0;
        h=0;
        extentMessage(FONT_HEADLINE,"Players Win",w,h);
        drawMessage(FONT_HEADLINE,"Players Win",(screenw-w)/2,maptop+120);
    } else if(count==0) {
        w=0;
        h=0;
        extentMessage(FONT_HEADLINE,"Players Lose",w,h);
        drawMessage(FONT_HEADLINE,"Players Lose",(screenw-w)/2,maptop+120);
    } else {
        int i=0;
        for(PlayerList::iterator p=playerList.begin();p!=playerList.end();p++) {
            Player *player=*p;
            i++;
            sprintf(buf,"%d:%d",i,player->health);
            if(player->isAlive())
                drawMessage(FONT_BODY,buf,32*i,maptop+220);
        }
    }
}

void Game::handleAction(int which,int id,bool down)
{
    if(playerList.size()>=1 && id>=0 && (unsigned int)which<playerList.size()) {
        Player *player=playerList[which];
        player->handleAction(id,down);
    }
    printf("id = %d  \n", id);
}

void Game::setMode(int newMode)
{
    mode=newMode;
}

bool Game::playersWin()
{
    int id=characterLayer->getTile(8,2);
    int count=0;

    for(ActorList::iterator p=enemyList.begin();p!=enemyList.end();p++) {
        Actor *enemy=*p;
        if(!enemy->enemy) continue;
        if(enemy->isAlive()) count++;
    }

    // dragon has spawned and enemies are vanquished.
    if(id==-1 && count==0) return true;

    return false;
}

bool Game::enemiesActive()
{
    int count=0;

    for(ActorList::iterator p=enemyList.begin();p!=enemyList.end();p++) {
        Actor *enemy=*p;
        if(enemy->isAlive()) count++;
    }

    return count>0;
}

bool Game::canMoveTo(Player *player,int tx,int ty)
{
    // check for map obstacles.
    int id=bgLayer->getTile(tx,ty);
    if(id==-1) return true;
    switch(id) {
    default:
        return false;
    case 0:     // grass
    case 5:     // water???
    case 23:    // stone tile
    case 30:    // wood floor
    case 33:    // dirt road 1
    case 34:    // dirt road 2
        break;
    }
    // check foreground
    id=fgLayer->getTile(tx,ty);
    if(id!=-1) {
        if(isObstacle(tx,ty)) return false;    // solid barrier
    }
    // check for enemy
    for(ActorList::iterator p=enemyList.begin();p!=enemyList.end();p++) {
        Actor *enemy=*p;

        if((enemy->tx==tx || enemy->tx+enemy->avatarWidth-1==tx) &&
           (enemy->ty==ty || enemy->ty+enemy->avatarHeight-1==ty) &&
           enemy->health>0) return false;
    }
    // check for other player
    for(PlayerList::iterator p=playerList.begin();p!=playerList.end();p++) {
        Player *other=*p;
        if(other==player) continue;

        if(other->tx==tx && other->ty==ty && other->health>0) return false;
    }

    // Stay with your friends
    if(ty*tile->tileHeight<maptop ||ty*tile->tileHeight>maptop+screenh) return false;

    return true;
}

bool Game::isObstacle(int tx,int ty)
{
    // check for map obstacles.
    int id=fgLayer->getTile(tx,ty);
    switch(id) {
    case 31:    // gate door
    case 45:    // fish
        return false;
    case 12:    // secret exit
    case 21:    // goblet on table
    case 32:    // building door
    case 22:    // candles
        if(enemyOnScreen()) return true;   // need to defeat enemies first
        return false;
    case 20:    // chair
        return false;
    }

    return false;
}

int Game::canCollect(int tx,int ty)
{
    // check for map obstacles.
    int id=fgLayer->getTile(tx,ty);
    switch(id) {
    case 31:    // gate door
    case 45:    // fish
        break;
    case 12:    // secret exit
    case 21:    // goblet on table
    case 32:    // building door
    case 22:    // candles
        if(enemyOnScreen()) return -1;   // need to defeat enemies first
        break;
    case 20:    // chair
        return -1;
    }

    return id;
}

int Game::collect(int tx,int ty)
{
    int id=canCollect(tx,ty);
    if(id==-1) return -1;
    fgLayer->setTile(tx,ty,-1);

    return id;
}

void Game::spawnEnemies()
{
    int tileY=maptop/16+1;

    for(int y=tileY;y<tileY+15;y++) {
        for(int x=0;x<characterLayer->tw;x++) {
            int id=characterLayer->getTile(x,y);

            if(id==-1) continue;
            characterLayer->setTile(x,y,-1);

            Actor *enemy=0;
            switch(id) {
            default:
                continue;
            case 60:    // goblin 1
            case 61:    // goblin 2
            case 63:    // skeleton
            case 76:    // dragon
                enemy=new Actor(tile);
                enemy->enemy=true;
                enemy->attackType=AT_FIRE;
                enemy->resetGame(x,y);
                break;
            case 43:    // small girl
            case 44:    // woman
            case 55:    // old man
            case 56:    // king
                enemy=new Actor(tile);
                enemy->enemy=false;
                enemy->attackType=AT_FIRE;
                enemy->resetGame(x,y);
                break;
            }


            if(id==76) {    // dragon
                enemy->avatarWidth=2;
                enemy->avatarHeight=2;
                characterLayer->setTile(x+1,y,-1);
                characterLayer->setTile(x,y+1,-1);
                characterLayer->setTile(x+1,y+1,-1);
                enemy->fullHealth=200;
                enemy->health=enemy->fullHealth;
            } else if(id==70) {    // hobgoblin
                enemy->avatarWidth=2;
                enemy->avatarHeight=2;
                characterLayer->setTile(x+1,y,-1);
                characterLayer->setTile(x,y+1,-1);
                characterLayer->setTile(x+1,y+1,-1);
                enemy->fullHealth=100;
                enemy->health=enemy->fullHealth;
            } else if(id==63) { // skeleton
                enemy->fullHealth=30;
                enemy->health=enemy->fullHealth;
            } else {    // goblins etc.
                enemy->fullHealth=20;
                enemy->health=enemy->fullHealth;
            }
            enemy->avatarId=id;
            enemy->avatarDeadId=-1;

            enemyList.push_back(enemy);
        }
    }
}

Player *Game::targetPlayer(Actor *enemy)
{
    if(playerList.empty()) return 0;

    Player *closest=0;
    float closestDist=0;
    for(PlayerList::iterator p=playerList.begin();p!=playerList.end();p++) {
        Player *player=*p;

        if(!player->isAlive()) continue;

        if(!closest) {
            float dx,dy;
            dx=player->tx-enemy->tx;
            dy=player->ty-enemy->ty;
            closestDist=dx*dx+dy*dy;
            if(enemy->avatarId==76) {
                if(closestDist>15*15) continue;
            } else {
                if(closestDist>8*8) continue;
            }

            closest=player;
            continue;
        }
        float dist;
        float dx,dy;
        dx=player->tx-enemy->tx;
        dy=player->ty-enemy->ty;
        dist=dx*dx+dy*dy;
        if(dist>8*8) continue;
        if(dist<closestDist) {
            closest=player;
            closestDist=dist;
        }
    }

    return closest;
}

Actor *Game::targetEnemy(Player *player)
{
    if(enemyList.empty()) return 0;

    Actor *closest=0;
    float closestDist=0;
    for(ActorList::iterator p=enemyList.begin();p!=enemyList.end();p++) {
        Actor *enemy=*p;

        if(!enemy->enemy) continue;
        if(!enemy->isAlive()) continue;

        if(!closest) {
            float dx,dy;
            dx=player->tx-enemy->tx;
            dy=player->ty-enemy->ty;
            closestDist=dx*dx+dy*dy;
            if(closestDist>10*10) continue;
            closest=enemy;

            continue;
        }
        float dist;
        float dx,dy;
        dx=player->tx-enemy->tx;
        dy=player->ty-enemy->ty;
        dist=dx*dx+dy*dy;
        if(dist<closestDist) {
            closest=enemy;
            closestDist=dist;
        }
    }

    return closest;
}

bool Game::enemyOnScreen()
{
    if(enemyList.empty()) return false;

    for(ActorList::iterator p=enemyList.begin();p!=enemyList.end();p++) {
        Actor *enemy=*p;

        if(!enemy->enemy) continue;
        if(!enemy->isAlive()) continue;

        if(enemy->ty*tile->tileHeight>maptop &&
           enemy->ty*tile->tileHeight<=maptop+240)
            return true;
    }
    return false;
}

void D(const char *format,const char *param) {
        FILE *log=fopen("log.txt","a");
        if(log) {
            fprintf(log,format,param);
            fclose(log);
        }
}

void D(const char *format,int param) {
        FILE *log=fopen("log.txt","a");
        if(log) {
            fprintf(log,format,param);
            fclose(log);
        }
}

void D(const char *format,float param) {
        FILE *log=fopen("log.txt","a");
        if(log) {
            fprintf(log,format,param);
            fclose(log);
        }
}

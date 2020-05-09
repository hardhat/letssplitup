#include <SDL/SDL.h>
#ifdef __APPLE__
#include <SDL_image/SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif

#include "hud.h"
#include "falling.h"
#include "sprite.h"

extern SDL_Surface *screen;

Hud::Hud()
{
	numbers=IMG_Load("data/numbers.png");
	victory[0]=IMG_Load("data/RainbowWins.png");
	victory[1]=IMG_Load("data/WhiteWins.png");
    victory[2]=IMG_Load("data/BlackWins.png");
    victory[3]=IMG_Load("data/BrownWins.png");
	icon[0]=IMG_Load("data/RainbowIcon.png");
	icon[1]=IMG_Load("data/WhiteIcon.png");
	icon[2]=IMG_Load("data/BlackIcon.png");
	icon[3]=IMG_Load("data/BrownIcon.png");
	who=0;
	whoX=0;
	whoY=0;
	for(int i=0;i<4;i++) {
    	score[i]=0;
    	heroX[i]=(screen->w-128)*i/4+64;
    	heroY[i]=0;
    }
	leftActive=false;
	rightActive=false;
	jumpActive=false;
}

void Hud::update(SpriteList &spriteList)
{
    int i;
    SpriteList::iterator p;

    i=0;    
    for(p=spriteList.begin();p!=spriteList.end();p++) {
        Sprite *hero=*p;
    	score[i]=hero->score;
    	i++;
    }

/*	
	for(i=0;i<4;i++) {
        int xx=(screen->w-64)*i/4;
        if(heroX[i]<xx) {
        	heroX[i]+=5;
        	if(heroX[i]>xx) heroX[i]=xx;
        } else {
            heroX[i]-=5;
            if(heroX[i]<xx) heroX[i]=xx;
        }
    	heroY[i]-=5;
    	if(heroY[i]<0) heroY[i]=0;
    }
    for(SpriteList::iterator p=p.begin();p.end();p++) {
        Sprite *hero=*p;
    	score[i]=hero->score;
    }
	i=0;
    for(p=spriteList.begin();p!=spriteList.end();p++,i++) {
        Sprite *hero=*p;
        
        if(who==hero) {
    		heroX[i]=whoX;
    		heroY[i]=whoY;
    		who=0;	
    	}
    }
    */
}

void Hud::animateScore(int viewx,int viewy,Falling *who)
{
	this->who=who;
	this->whoX=who->x-viewx;
	this->whoY=who->y-viewy;	
}

void Hud::drawDigit(int x,int y,int i)
{
	SDL_Rect src={static_cast<Sint16>(i*32),0,32,32};
	SDL_Rect dest={static_cast<Sint16>(x),static_cast<Sint16>(y),32,32};
	SDL_BlitSurface(numbers,&src,screen,&dest);
}

void Hud::drawArrow(int x,int y,int i)
{
	drawDigit(x,y,i+10);
}

void Hud::drawIcon(int x,int y,SDL_Surface *image)
{
	SDL_Rect dest={static_cast<Sint16>(x),static_cast<Sint16>(y),0,0};
	SDL_BlitSurface(image,NULL,screen,&dest);
}

void Hud::drawDigit(SDL_Surface *icon,int x,int y,int i)
{
	drawIcon(x,y,icon);
	drawDigit(x+32,y,i);
}

void Hud::draw()
{
    int i;
    for(i=0;i<4;i++) {
    	if(score[i]>=9) {
            if(victory[i]) drawIcon(screen->w/2-victory[i]->w/2,100,victory[i]);
    	}
    }
    for(i=0;i<4;i++) {
    	drawDigit(icon[i],(int)heroX[i],(int)heroY[i],score[i]);
    }
	if(leftActive) drawArrow(10,screen->h/2,3);
	drawArrow(10,screen->h/2,0);
	if(rightActive) drawArrow(screen->w-32-10,screen->h/2,3);
	drawArrow(screen->w-32-10,screen->h/2,1);
	if(jumpActive) drawArrow(screen->w/2-16,screen->h-10-32,3);
	drawArrow(screen->w/2-16,screen->h-10-32,2);
}

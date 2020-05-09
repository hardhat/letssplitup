#include <stdio.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "main.h"
#include "font.h"

TTF_Font *font[4];

void initFont()
{
	TTF_Init();

	font[0]=TTF_OpenFont("data/Oswald-Bold.ttf",36);
	font[1]=TTF_OpenFont("data/Oswald-Regular.ttf",14);
	font[2]=TTF_OpenFont("data/Oswald-Regular.ttf",18);
	font[3]=TTF_OpenFont("data/Oswald-Regular.ttf",14);
	printf("Font status: %s, %s, %s\n",font[0]?"yes":"no",font[1]?"yes":"no",font[2]?"yes":"no");
}

void extentMessage(FontId fontId,const char *message,int &w,int &h)
{
	w=0;
	h=0;

	if(!font[fontId]) return;
	TTF_SizeText(font[fontId],message,&w,&h);
}

void drawMessage(FontId fontId,const char *message,int x,int y)
{
	SDL_Surface *surf;
	SDL_Rect rect;
	SDL_Color fg={255,255,255,255};
	if(fontId==2) {
		fg.g=96;
		fg.r=96;
	} else if(fontId==0) {
		fg.b=128;
	} else if(fontId==3) {
		fg.b=96;
		fg.g=128;
	}

	if(!font[fontId]) {
		static int count=0;
		if(count<30)
			printf("message at %d,%d: '%s'\n",x,y,message);
		count++;
		return;
	}
	surf=TTF_RenderText_Blended(font[fontId],message,fg);
	rect.x=x*renderScale+screenleft;
	rect.y=(y-maptop)*renderScale+screentop;
	rect.w=surf->w*renderScale;
	rect.h=surf->h*renderScale;
	//SDL_SetColorKey(surf, SDL_SRCCOLORKEY, 0);
	SDL_Texture* texture = NULL;
    texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
}

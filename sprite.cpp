#include <stdio.h>

#include <map>
#include <string>

#include <SDL.h>
extern "C" {
#include <SDL_image.h>
}

#include "main.h"
#include "sprite.h"

struct Texture {
	int w,h;
	SDL_Texture *texture;
};

typedef std::map<std::string,int> SpriteNameMap;
typedef std::map<int,Texture *> SpriteTextureMap;

static SpriteNameMap nameMap;
static SpriteTextureMap textureMap;
static int spriteId;

Sprite::Sprite(const char *name)
{
	x=0;
	y=0;
	w=0;
	h=0;
	imageId=-1;
	if(nameMap.find(name)!=nameMap.end()) {
		imageId=nameMap[name];
		w=textureMap[imageId]->w;
		h=textureMap[imageId]->h;
		return;
	}
	std::string path=(std::string)"data/"+name;
	SDL_Surface *bitmap=IMG_Load(path.c_str());

	if(!bitmap) {
		fprintf(stderr,"Can't load image '%s'\n",path.c_str());
		return;
	}

    SDL_Texture* tex = NULL;
    tex = SDL_CreateTextureFromSurface(renderer, bitmap);
	Texture *texture=new Texture();
	texture->w=bitmap->w;
	texture->h=bitmap->h;
	texture->texture=tex;
	x=0;
	y=0;
	w=texture->w;
	h=texture->h;
    SDL_FreeSurface(bitmap);

    textureMap[spriteId]=texture;
    nameMap[name]=spriteId;
    imageId=spriteId;
    spriteId++;
}

Sprite::~Sprite()
{

}

void Sprite::update(int elapsed)
{
}

void Sprite::draw()
{
	if(textureMap.find(imageId)==textureMap.end()) {
		SDL_Rect rect={(int)((x-8)*renderScale)+screenleft,(int)((y-8-maptop)*renderScale)+screentop,(int)(16*renderScale),(int)(16*renderScale)};
		static int color=0;
		Uint8 r=255*color/32, g=255, b=255;
		color=(color+1)%32;
		SDL_SetRenderDrawColor(renderer,r,g,b,255);
		SDL_RenderFillRect(renderer,&rect);
		return;
	}

	Texture *t=textureMap[imageId];
	SDL_Rect rect={(int)((x-t->w/2)*renderScale)+screenleft,(int)((y-t->h/2-maptop)*renderScale)+screentop,(int)(t->w*renderScale),(int)(t->h*renderScale)};
	SDL_RenderCopy(renderer, t->texture, NULL, &rect);
}

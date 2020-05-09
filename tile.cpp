#include <stdio.h>
#include "tile.h"

#include <SDL.h>
extern "C" {
#include <SDL_image.h>
}

#include <map>

#include "main.h"
#include "sprite.h"

struct Texture {
	int w,h;
	SDL_Texture *texture;
};

typedef std::map<std::string,int> TileNameMap;
typedef std::map<int,Texture *> TileTextureMap;

static TileNameMap tileNameMap;
static TileTextureMap tileTextureMap;
static int tileId;

Tile::Tile(const char *name,int tileWidth,int tileHeight)
{
	this->tileWidth=tileWidth;
	this->tileHeight=tileHeight;
	imageId=-1;
	if(tileNameMap.find(name)!=tileNameMap.end()) {
		imageId=tileNameMap[name];
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
    SDL_FreeSurface(bitmap);

    tileTextureMap[tileId]=texture;
    tileNameMap[name]=tileId;
    imageId=tileId;
    tileId++;
}

Tile::~Tile()
{

}

void Tile::draw(int id,int col,int row)
{
    int x=col*tileWidth;
    int y=row*tileHeight;
    if(id<0) return;    // blank spot
   	if(tileTextureMap.find(imageId)==tileTextureMap.end()) {
		SDL_Rect rect={(int)((x-8)*renderScale)+screenleft,(int)((y-8-maptop)*renderScale)+screentop,(int)(tileWidth*renderScale),(int)(tileHeight*renderScale)};
		static int color=0;
		Uint8 r=255*color/32, g=255, b=255;
		color=(color+1)%32;
		SDL_SetRenderDrawColor(renderer,r,g,b,255);
		SDL_RenderFillRect(renderer,&rect);
		return;
   	}

	Texture *t=tileTextureMap[imageId];
	int tilesAcross = t->w/tileWidth;
	int tx=id%tilesAcross;
	int ty=id/tilesAcross;
	SDL_Rect srcRect={tx*tileWidth,ty*tileHeight,tileWidth,tileHeight};
	SDL_Rect rect={(int)((x-tileWidth/2)*renderScale)+screenleft,(int)((y-tileHeight/2-maptop)*renderScale)+screentop,(int)(tileWidth*renderScale),(int)(tileHeight*renderScale)};
    if(rect.x>screenw || rect.y>screenh || rect.x-rect.w<0 || rect.y-rect.h<0) return;
	SDL_RenderCopy(renderer, t->texture, &srcRect, &rect);
}

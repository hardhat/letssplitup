#include <deque>
#include <stdio.h>
#include "map.h"
#include "falling.h"

#ifndef _PSP
extern SDL_Surface *screen;
#else
struct Screen { int w,h; } scr={480,272};
struct Screen *screen;
#endif

#define T_SKY 4
#define T_LADDER 2
#define T_GROUND 0
#define T_GROUNDLADDER 6
#define T_CEILING 3
#define T_LADDERTOP 7
#define T_ROCK 1
#define T_CLIFF 8

class TileCollide {
public:
	int id;
	bool solid;
	bool climbable;
	bool obstacle;
} tileCollide[]={
	{ T_SKY, false, false, false },
	{ T_LADDER, false, true, false },
	{ T_LADDERTOP, true, true, false },
	{ T_GROUNDLADDER, true, true, false },
	{ T_GROUND, true, false, false },
	{ T_ROCK, true, false, true },
	{ T_CEILING, true, false, true },
	{ T_CLIFF, true, false, false },
	{ -1, false, false}
};


Map::Map()
{
	viewx=0;
	viewy=0;
	backgroundImage=0;
	levelImage=0;
	collisionImage=0;
	tilesAcross=30;
	tilesDown=15;
	cell=new int[tilesAcross*tilesDown];
	gradient=new int[tilesAcross*tilesDown];
	cellw=64;
	cellh=64;
	viewTimer=1000;
#ifdef _PSP
	screen=&scr;
#else
	screen=::screen;
#endif
}

Map::~Map()
{
#ifdef _PSP
	if(backgroundImage) oslDeleteImage( backgroundImage);
	if(levelImage) oslDeleteImage( levelImage);
	if(collisionImage) oslDeleteImage( collisionImage);
#else
	if(backgroundImage) SDL_FreeSurface( backgroundImage);
	if(levelImage) SDL_FreeSurface( levelImage);
	if(collisionImage) SDL_FreeSurface( collisionImage);
#endif
	delete []cell;
	cell=0;
	delete []gradient;
	gradient=0;
}

void Map::load(const char *level)
{
	this->level=level;
#ifdef _PSP
	backgroundImage=oslLoadImageFile((char *)"data/sky1.png",OSL_IN_RAM,OSL_PF_8888);
	levelImage=oslLoadImageFile((char *)"data/random_0.png",OSL_IN_RAM,OSL_PF_8888);
	collisionImage=oslLoadImageFile((char *)"data/collision.png",OSL_IN_RAM,OSL_PF_8888);
#else
	printf("Loading..\n");
	backgroundImage=IMG_Load("data/background.png");
	levelImage=IMG_Load("data/random_0.png");
	collisionImage=IMG_Load("data/collision.png");
#endif
	int i,j;
	for(j=0;j<tilesDown;j++) {
		for(i=0;i<tilesAcross;i++) {
			cell[i+j*tilesAcross]=0;
		}
	}
	FILE *file=fopen(level,"r");
	if(!file) return;
	for(j=0;j<tilesDown;j++) {
		char line[1024];
		if(!fgets(line,1023,file) ) {
			break;
		}
		for(i=0;i<tilesAcross;i++) {
			if(line[i]==0) break;
			int pos=i+j*tilesAcross;
			switch(line[i]) {
			case ' ': cell[pos]=T_SKY; break;
			case 'L': cell[pos]=T_LADDER; break;
			case 'F': cell[pos]=T_LADDERTOP; break;
			case 'B': cell[pos]=T_GROUNDLADDER; break;
			case '.': cell[pos]=T_GROUND; break;
			case 'R': cell[pos]=T_ROCK; break;
			case '_': cell[pos]=T_CEILING; break;
			case '#': cell[pos]=T_CLIFF; break;
			}
		}
	}
	fclose(file);
	printf("Done..\n");
}

TileCollide *Map::getCollide( int tile)
{
	int i;
	for(i=0;tileCollide[i].id>-1;i++) {
		if(tileCollide[i].id==tile) {
			return tileCollide+i;
		}
	}
	return 0;
}

bool Map::collide( Falling *object, float newx, float newy)
{
	int cx=(int)newx/cellw;
	int cy=((int)(newy+object->height/2)/cellh);
	int pos=cx+cy*tilesAcross;

	if(cx<0 || cx>=tilesAcross || cy<0 || cy>=tilesDown) {
		return true;
	}

	TileCollide *tc=getCollide(cell[pos]);
	if(tc->solid) {	
		float ry=(newy+object->height/2)-cy*cellh;
		//printf("y=%.2f, cy=%d, ry=%.2f\n",newy,cy,ry);
		if(ry<8) {
			if(tc->obstacle) return true;
			return false;
		}
		return true;
	}
	if(tc->climbable) return false;

	return false;
}

void Map::drawTile(int tile, int x, int y)
{
	int top,left;
	left=(tile%10)*cellw;	// convert from tiles across to pixels across.
	top=(tile/10)*cellh;	// convert from tiles down to pixels down.
	int dx=x-viewx,dy=y-viewy;

	// clipping outside of screen.
	if(dx+cellw<0 || dx>screen->w || dy+cellh<0 || dy>screen->h) return;
	// skip empty tiles
	if(tile==T_SKY) return;
#ifdef _PSP
	oslSetImageTileSize(levelImage,left,top,cellw,cellh);	// set the sprite sheet up to show that sprite.
	levelImage->x=dx;	// draw it where it goes
	levelImage->y=dy;	// draw it where it goes
	oslDrawImage(levelImage);
#else
	SDL_Rect src={static_cast<Sint16>(left),static_cast<Sint16>(top),static_cast<Uint16>(cellw),static_cast<Uint16>(cellh)},dest={static_cast<Sint16>(dx),static_cast<Sint16>(dy),static_cast<Uint16>(cellw),static_cast<Uint16>(cellh)};
	SDL_BlitSurface( levelImage, &src, screen, &dest);
#endif
}

void Map::draw()
{
#ifdef _PSP
	oslDrawImage(backgroundImage);
#else
	if(backgroundImage) {
		SDL_Rect src={0,0,static_cast<Uint16>(screen->w),static_cast<Uint16>(screen->h)};
		int srangex=cellw*tilesAcross-screen->w;
		int srangey=cellh*tilesDown-screen->h;
		int drangex=backgroundImage->w-screen->w;
		int drangey=backgroundImage->h-screen->h;
		src.x=srangex>0?viewx*drangex/srangex:0;
		src.y=srangey>0?viewy*drangey/srangey:0;
//		SDL_BlitSurface(backgroundImage,&src,screen,0);
	}
#endif
	int i,j;
	for(j=0;j<tilesDown;j++) {
		for(i=0;i<tilesAcross;i++) {
			drawTile(cell[i+j*tilesAcross],i*cellw,j*cellh);
		}
	}
}

void Map::updatePhysics()
{

}

void Map::updateView(Falling *hero)
{
	// horizontal
	viewx=(int)((hero->x-screen->w/2)*0.02f+viewx*0.98f);
	if( hero->x-screen->w/3<viewx) {
		viewx=(int)hero->x-screen->w/3;
	}
	if( hero->x-screen->w*2/3>viewx) {
		viewx=(int)hero->x-screen->w*2/3;
	}
	if( viewx<-32 ) viewx=-32;
	if( viewx>tilesAcross*cellw-screen->w+32) {
		viewx=tilesAcross*cellw-screen->w+32;
	}
	// vertical
	viewy=(int)((hero->y+screen->h/2)*0.02f+viewy*0.98f);
	if( hero->y-screen->h/3<viewy) {
		viewy=(int)hero->y-screen->h/3;
	}
	if( hero->y-screen->h*2/3>viewy) {
		viewy=(int)hero->y-screen->h*2/3;
	}
	if( viewy<-128 ) viewy=-128;
	if( viewy>tilesDown*cellh-screen->h+64) {
		viewy=tilesDown*cellh-screen->h+64;
	}
}

void Map::updateView(Falling *hero, Falling *baddie)
{
	float realx=viewx,realy=viewy;
	
	if(viewTimer>1000) {
		updateView(hero);
	} else {
		updateView(baddie);
	}
	viewTimer-=16;
	if(viewTimer<=0) viewTimer=2000;
	viewx=(int)(0.05f*viewx+0.95f*realx);
	viewy=(int)(0.05f*viewy+0.95f*realy);
}

bool Map::gradientCollide(int tile)
{
	TileCollide *tc=getCollide(tile);
	if(tc->solid) {	
		return true;
	}
	if(tc->obstacle) return true;
	if(tc->climbable) return false;

	return false;
}

class SearchGrad { 
public:
    int x,y,id;
    SearchGrad(int xx,int yy,int idid) {
        x=xx;
        y=yy;
        id=idid;
    }
};

void Map::setGradient(int cx,int cy,int id)
{
	int i;
    //const int off[8][2]={ {-1,0},{-1,-1},{0,-1},{1,-1}, {1,0},{1,1},{0,1},{-1,1} };
    const int off[4][2]={ {-1,0},{0,-1}, {1,0},{0,1} };
    std::deque<SearchGrad *> list;
    int active[tilesAcross*tilesDown];
    for(i=0;i<tilesAcross*tilesDown;i++) active[i]=4096;
    
    list.push_back(new SearchGrad(cx,cy,id));
    
    int count=0;
    // Breadth first fill
    while(list.size()>0) {
        SearchGrad *g=list.front();
        list.pop_front();
        count++;
        cx=g->x;
        cy=g->y;
        id=g->id;
        delete g;
        g=0;
    	if(cx<0 || cx>=tilesAcross || cy<0 || cy>=tilesDown) {
    		continue;
    	}
        if(gradientCollide(cell[cx+cy*tilesAcross])) continue; // can't go here anyway.
    	if(gradient[cx+cy*tilesAcross]<id) continue; // already visited here.
    	gradient[cx+cy*tilesAcross]=id;
    	//printf("Queueing at %d,%d id=%d\n",cx,cy,id);
    	for(i=0;i<4;i++) {
            int pos=(cx+off[i][0])+(cy+off[i][1])*tilesAcross;
            if(active[pos]!=4096) continue;
            active[pos]=id+1;
            list.push_back(new SearchGrad(cx+off[i][0],cy+off[i][1],id+1));
        }
    }
    printf("count: %d steps...(queue size=%d)\n",count,list.size());
    int j;
    for(j=0;j<tilesDown;j++) {
        for(i=0;i<tilesAcross;i++) {
            int ch=gradient[i+j*tilesAcross];
            if(ch<64) ch+=64;
            else if(ch==4096) ch='*';
            else ch='0'+ch/410;
            printf("%c",ch);
        }
        printf("\n");
    }
}

void Map::calculateGradient(Falling *target)
{
    int i,x,y;
    float newx=target->x;
    float newy=target->y;
    for(i=0;i<tilesAcross*tilesDown;i++) gradient[i]=4096;

   	int cx=(int)newx/cellw;
	int cy=((int)(newy+target->height/2)/cellh);
	int pos=cx+cy*tilesAcross;

	if(cx<0 || cx>=tilesAcross || cy<0 || cy>=tilesDown) {
		return;
	}
    
    setGradient(cx,cy,0);
}

int Map::gradientValue(float newx,float newy)
{
   	int cx=(int)newx/cellw;
	int cy=((int)(newy+16)/cellh);
	int pos=cx+cy*tilesAcross;

	if(cx<0 || cx>=tilesAcross || cy<0 || cy>=tilesDown) {
		return 4096;
	}

    return gradient[pos];
}

void Map::getDirection(Falling *baddie,int &dirx,int &diry)
{
    int minv=gradientValue(baddie->x,baddie->y);
    int mindir=-1,minx=0,miny=0;
    const int off[8][2]={ {-1,0},{-1,-1},{0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1} };
    int i;
    for(i=0;i<8;i++) {
        int dx=off[i][0]*cellw;
        int dy=off[i][1]*cellh;
        int v=gradientValue(baddie->x+dx,baddie->y+dy);
        if(v<minv) {
            minv=v;
            minx=dx;
            miny=dy;
            mindir=i;
        }
    }
    if(miny<0) diry=1;
    else diry=0;
    if(minx<0) dirx=-1;
    else if(minx>0) dirx=1;
    else dirx=0;
}

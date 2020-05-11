#include<stdio.h>
#include <SDL.h>
#include "game.h"
#include "tile.h"
#include "layer.h"

Layer::Layer(Tile *tile) {
    this->tile=tile;
    tileMap=0;
    th=0;
    tw=0;
}

Layer::~Layer() {

}

void Layer::load(const char *filename)
{
    tw=0;
    th=0;
    if(tileMap) delete tileMap;
    tileMap=0;
    // pass 1, measure size
    FILE *file=fopen(filename,"r");
    if(!file) {
        D("Fatal: file not found '%s'\n",filename);
        return;
    }

    int ch;
    int pos=0;
    while((ch=getc(file))!=EOF) {
        if(th==0) {
            if(ch>='0' && ch<='9' && tw==0) tw++;
            else if(ch==',') tw++;
        }
        if(ch!='\n' && ch!=' ' && ch!='\t') {
            pos++;
        } else if(ch=='\n') {
            pos=0;
            th++;
        }
    }
    fseek(file,0,SEEK_SET);
    tileMap=new int[tw*th];

    char buf[64];
    pos=0;
    int x=0,y=0;
    while((ch=getc(file))!=EOF) {
        if((ch>='0' && ch<='9')||ch=='-') {
            buf[pos++]=ch;
            buf[pos]=0;
        } else if(ch==',' || ch=='\n') {
            int id=0;
            pos=0;
            sscanf(buf,"%d",&id);
            setTile(x,y,id);
            printf("%d, ", id);
            x++;
            if(ch=='\n') {
                printf("\n");
                x=0;
                y++;
            }
        }
    }
    fclose(file);
}

void Layer::draw()
{
    if(!tileMap) return;

    for(int y=0;y<th;y++) {
        for(int x=0;x<tw;x++) {
            tile->draw(getTile(x,y),x,y);
        }
    }
}

int Layer::getTile(int x,int y) {
    if(x<0 || y<0 || x>=tw || y>=th) return -1;
    return tileMap[x+y*tw];
}

void Layer::setTile(int x,int y,int value) {
    if(x<0 || y<0 || x>=tw || y>=th) return;
    tileMap[x+y*tw]=value;
}
int Layer::getX(){
    //return x;
}

int Layer::gteY(){
    //return y;
}

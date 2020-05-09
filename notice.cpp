#include "main.h"
#include "notice.h"
#include "font.h"

Notice::Notice(float x,float y,const char *note)
{
	this->x=x;
	this->y=y;
	w=0;
	h=0;
	timer=1000;
	vy=-0.05;
	vx=0;
	message=note;
}

void Notice::draw() {
	drawMessage(FONT_NOTICE,message.c_str(),x,y);
}

void Notice::update(int elapsed) {
	if(timer>0) {
		x+=vx*elapsed;
		y+=vy*elapsed;
		if(timer>elapsed) timer-=elapsed; else timer=0;
	}
}

bool Notice::isActive()
{
	return timer>0;
}

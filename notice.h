#ifndef NOTICE_H
#define NOTICE_H

#include <string>

class Notice
{
public:
	float x,y;
	float w,h;
	float vx,vy;
	int timer;
	std::string message;

	Notice(float x,float y,const char *note);
	
	void draw();
	void update(int elapsed);
	bool isActive();
};

#endif


#ifndef SPRITE_H
#define SPRITE_H

#include <string>

class Sprite {
public:
	float x,y,w,h;
	int imageId;
	std::string name;
	
	Sprite(const char *name);
	~Sprite();
	void update(int elapsed);
	void draw();
};

#endif

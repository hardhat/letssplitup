#ifndef TILE_H
#define TILE_H

class Tile {
public:
	int tileWidth,tileHeight;
	int imageId;

	Tile(const char *filename,int tileWidth,int tileHeight);
	~Tile();
	void draw(int id,int col,int row);
};

#endif

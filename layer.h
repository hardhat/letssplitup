#ifndef LAYER_H
#define LAYER_H
class Tile;

class Layer {
public:
	int tw,th;

	Layer(Tile *tile);
	~Layer();
	void load(const char *filename);
	void draw();

	int getTile(int tx,int ty);
	void setTile(int tx,int ty,int value);

private:
	int *tileMap;
	Tile *tile;
};

#endif

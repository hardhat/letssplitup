#include <deque>

class Map;
class Sprite;
class Hud;
class Image;
typedef std::deque<Sprite *> SpriteList;
typedef std::deque<Image *> ImageList;

class Game
{
public:
    Game();
    bool hitTarget(Sprite *hero,Map &map,Hud &hud);
    bool hitTarget(Sprite *hero,Image *target,Map &map,Hud &hud);
    void mapReset(Map &map);
    void newGame(Map &map);

    void draw(Map &map,Hud &hud);
    void update(Map &map,Hud &hud);
    void handleDown(int key);
    void handleUp(int key);
    void addCharSprite(Sprite *spriteToAdd);
    void addItemImage(Image *imageToAdd);
    SpriteList _playerList;
    ImageList _itemList;
private:
    Map *_map;
//    SpriteList _playerList;
//    ImageList _itemList;
};


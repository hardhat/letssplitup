#ifndef GAME_H
#define GAME_H

#include <deque>

#include "layer.h"
#include "tile.h"

enum GameMode {
    MODE_TITLE,
    MODE_CHOOSEAVATAR,
    MODE_LEVEL = 1,
    MODE_GAMEOVER,
};

enum DPadKey {
    DPAD_LEFT = 0,
    DPAD_UP = 1,
    DPAD_RIGHT = 2,
    DPAD_DOWN = 3,
    DPAD_A = 4,
    DPAD_B = 5,
    DPAD_SELECT=6,
    DPAD_START = 7,
};

class Player;
class Actor;

typedef std::deque<Player *> PlayerList;
typedef std::deque<Actor *> ActorList;
typedef std::deque<Layer *> LayerList;

class Game {
public:
    int mode;
    Tile *tile;
    Layer *bgLayer;
    Layer *fgLayer;
    Layer *characterLayer;
    PlayerList playerList;
    ActorList enemyList;
    int totalTime;

    Game();
    ~Game();

    void resetGame();
    void setMode(int newMode);
    void update(int elapsed);
    void draw();
    void handleAction(int which,int id,bool down);

    bool playersWin();
    bool enemiesActive();
    bool canMoveTo(Player *player,int tx,int ty);
    bool isObstacle(int tx,int ty);
    int canCollect(int tx,int ty);
    int collect(int tx,int ty);
    void spawnEnemies();
    Actor *targetEnemy(Player *player);
    Player *targetPlayer(Actor *enemy);
    bool enemyOnScreen();
};

extern Game game;

void D(const char *format,const char *param);
void D(const char *format,int param);
void D(const char *format,float param);

#endif

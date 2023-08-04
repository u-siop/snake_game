#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_
#include "block.h"
#include "map.h"
#include "snake.h"

class gameManager{
private:

public :
    gameManager();
    ~gameManager();

    void init();
    void update();
    void render();
    void release();
};
#endif
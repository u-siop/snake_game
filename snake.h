#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include "block.h"
using namespace std;

class Snake : public block{
private:
    vector<Location> snake;

public:
    void direction();
    pair<int, int> getHeadLocation();
    
};
#endif
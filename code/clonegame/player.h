#pragma once
#include "textureManager.h"
#include <string>

class Player
{
  public:
    int x, y;
    float mvelX, mVely;
    int width;
    int height;
    Player(int posx, int posy): x(posx), y(posy) {};
    void draw(TextureManager *texManager);
};

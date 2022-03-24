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
    int numSprites;
    std::string name;
    Player(int posx, int posy, std::string resName): x(posx), y(posy), name(resName) {};
    void sampleFromSpritesheet(TextureManager *texManager, SDL_Texture* bitmapTex, int pos);
};

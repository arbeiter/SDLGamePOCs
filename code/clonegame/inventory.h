#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>
#include <ostream>
#include "textureManager.h"
#include <vector>
using namespace std;

struct ItemStats {
    int itemCount;
    string itemName;
    int top_left_x;
    int top_left_y;
    int width;
    int height;
};

class Inventory {
  public:
    Inventory() = default;
    Inventory(SDL_Renderer *renderer, TTF_Font *mFont)
      : mRenderer{renderer}, font(mFont), width(3440), height(1440)
    {
      texManager = new TextureManager(renderer);
      bitmapTex = texManager->LoadTexture("./res/final_collectables_transparent.png");
      loadItemStats();
      int wi, hi;
      SDL_QueryTexture(bitmapTex, NULL, NULL, &wi, &hi);
      tex_w = wi;
      tex_h = hi;
      highlighted_index = -1;
    }

    void draw();
    void refreshDisplay();
    void loadItemStats();
    void drawItem(SDL_Rect &srcrect, int width, int item_idx);
    void drawCount(SDL_Rect &srcrect, int count);
    void computeIntersection(int x, int y);

  private:
    void drawChrome();
    void drawItems(int w, int h);
    void drawItem();
    int highlighted_index;
    SDL_Renderer *mRenderer;
    int width;
    int height;
    TextureManager *texManager;
    SDL_Texture *bitmapTex;
    int tex_w;
    int tex_h;
    std::vector<ItemStats> itemStats;
    TTF_Font* font;
};

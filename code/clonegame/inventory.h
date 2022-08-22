#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>
#include <ostream>
#include "textureManager.h"
#include <vector>
using namespace std;


struct MouseState {
  float sourceX;
  float sourceY;
  float destinationX;
  float destinationY;
  bool dragMode;
  int selectedIndex;
};

struct ItemStats {
    int itemCount;
    string itemName;
    int top_left_x;
    int top_left_y;
    int width;
    int height;
};
struct ItemDropLocations {
  float drop_x;
  float drop_y;
  int item_key;
};

class Inventory {
  public:
    bool firstLoad;
    Inventory() = default;
    std::vector<ItemDropLocations> dropLocations;
    Inventory(SDL_Renderer *renderer, TTF_Font *mFont)
      : mRenderer{renderer}, font(mFont), width(3440), height(1440)
    {
      loadItemStats();
      texManager = new TextureManager(renderer);
      bitmapTex = texManager->LoadTexture("./res/final_collectables_transparent.png");
      int wi, hi;
      SDL_QueryTexture(bitmapTex, NULL, NULL, &wi, &hi);
      tex_w = wi;
      tex_h = hi;
      highlighted_index = -1;
      //mouseState = { -1, -1, -1, -1, false, -1 };
      firstLoad = false;
    }

    void draw();
    void refreshDisplay();
    void loadItemStats();
    void drawDropLocations();
    void drawInventoryItem(int x, int y, int index);
    void drawItem(SDL_Rect &srcrect, int width, int item_idx);
    void drawItemInner(SDL_Rect &srcrect, int width, int item_idx, bool isTextured);
    void renderTextureWithTransform(SDL_Rect &dstrect);
    void drawCount(SDL_Rect &srcrect, int count);
    int computeIntersection(int x, int y);
    MouseState setMouseState(MouseState mouseState);
    void itemDraggable();
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
    MouseState mouseState;
    SDL_Texture *texTarget;
};

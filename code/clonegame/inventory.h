#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <ostream>
#include "textureManager.h"
using namespace std;

class Inventory {
  public:
    Inventory(SDL_Renderer *renderer)
      : mRenderer{renderer}, width(3440), height(1440), num_items(60)
    {
      texManager = new TextureManager(renderer);
      bitmapTex = texManager->LoadTexture("./res/collectable_jam.png");

      int wi, hi;
      SDL_QueryTexture(bitmapTex, NULL, NULL, &wi, &hi);
      tex_w = wi;
      tex_h = hi;
    }
    void draw();
    void refreshDisplay();

  private:
    void drawChrome();
    void drawItems(int w, int h);
    SDL_Renderer *mRenderer;
    int width;
    int height;
    int num_items;
    TextureManager *texManager;
    SDL_Texture *bitmapTex;
    int tex_w;
    int tex_h;
};

class InventoryItem {
  public:
    InventoryItem(SDL_Renderer *renderer)
                  : mRenderer{renderer} {}
  private:
    void drawChrome();
    void drawInterior();
    int padding;
    string item_texture;
    string item_count;
    string border_texture;
    SDL_Renderer *mRenderer;
};

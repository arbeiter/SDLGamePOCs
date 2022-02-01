#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>

class TextureManager {
  public:
    TextureManager(SDL_Renderer *renderer)
      : mRenderer{renderer}
    {}

    void RenderTexture(SDL_Texture *tex, int x, int y);
    void RenderTexture(SDL_Texture *tex, int x, int y, int w, int h); 
    SDL_Texture* LoadTexture(const char* texture);
    void ClipTexture(SDL_Texture *tex, float x, float y, int w, int h, int p_x, int p_y, int p_w, int p_h); 

  private:
    SDL_Renderer *mRenderer;
};

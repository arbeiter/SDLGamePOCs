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
    void ClipTexture(SDL_Texture *tex, int x, int y, int w, int h, int dest_w, int dest_h); 

  private:
    SDL_Renderer *mRenderer;
};

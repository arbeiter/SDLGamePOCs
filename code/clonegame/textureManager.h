#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class TextureManager {
  public:
    TextureManager(SDL_Renderer *renderer)
      : mRenderer{renderer}
    {}

    void RenderTexture(SDL_Texture *tex, int x, int y);
    void RenderTexture(SDL_Texture *tex, int x, int y, int w, int h); 
    SDL_Texture* LoadTexture(const char* texture);

  private:
    SDL_Renderer *mRenderer;
};

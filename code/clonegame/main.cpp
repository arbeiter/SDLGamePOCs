#include "game.h"
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>

void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main(int argc, char *argv[])
{
  signal(SIGSEGV, handler);   // install our handler
  Game game;
  std::cout << "Start game";
  bool success = game.initialize();
  if (success) {
    game.runLoop();
  }
  game.shutDown();
  return 0;
}

void alternative_debugging_window() {
  const int WIN_WIDTH = 640;
  const int WIN_HEIGHT = 480;
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
    return;
  }
  SDL_Window *win = SDL_CreateWindow("Rendering to a texture!", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

  TextureManager texManager = TextureManager(renderer);
  SDL_Texture *bmpTex = texManager.LoadTexture("./res/grass_texture_seamless.jpeg");


  //Make a target texture to render too
  SDL_Texture *texTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                           SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);

  //Now render to the texture
  SDL_SetRenderTarget(renderer, texTarget);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, bmpTex, NULL, NULL);

  SDL_Rect srcrect;
  srcrect.x = (int)(WIN_WIDTH/3);
  srcrect.y = (int)(WIN_HEIGHT/3);
  srcrect.w = (int)(10);
  srcrect.h = (int)(WIN_HEIGHT/3);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
  SDL_RenderFillRect(renderer, &srcrect);

  //Detach the texture
  SDL_SetRenderTarget(renderer, NULL);

  //Now render the texture target to our screen, but upside down
  SDL_RenderClear(renderer);
  SDL_RenderCopyEx(renderer, texTarget, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL);
  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(texTarget);
  SDL_DestroyTexture(bmpTex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
}

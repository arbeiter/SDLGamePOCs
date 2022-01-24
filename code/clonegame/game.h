#pragma once
#include "SDL2/SDL.h"

class Game
{
  public:
    Game();
    bool initialize();
    void runLoop();
    void shutDown();

  private:
    void processInput();
    void updateGame();
    void generateOutput();

  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
};

#pragma once
#include "SDL2/SDL.h"
#include <ostream>
#include <iostream>

struct Vector2
{
	float x;
	float y;
};

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
    
    bool mIsRunning;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    Vector2 mPaddlePos;
    Vector2 mBallPos;
};

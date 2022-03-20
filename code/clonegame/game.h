#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <ostream>
#include <iostream>
#include "player.h"

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
    void FillScreenWithGrass();
    void WallLayer(); 
    void DrawActor();
    bool get_intersection(int a_x, int a_y, int b_x, int b_y); 

    bool mIsRunning;
    Player mPlayer;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    int mTicksCount;
    SDL_Texture* LoadTexture(const char* fileName);
    void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};

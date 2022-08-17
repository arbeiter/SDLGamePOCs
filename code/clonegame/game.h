#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <ostream>
#include <iostream>
#include "player.h"
#include "inventory.h"

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
    std::string debug_string;
    TTF_Font *_font;
    int rect_height = 0;
    int rect_width = 0;
    int num_rows = 11;
    int num_cols = 16;
    int collectable_backing[11][16] = {{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 1,0,0,0, 0,0,0,0, 0,0,0,0},
                                       {0,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,0,0}};
    const int rr[11][16] = {{0,0,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1},
                            {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                            {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                            {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                            {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                            {0,1,0,0, 1,0,0,0, 1,0,0,0, 0,0,0,0},
                            {0,1,0,0, 1,0,0,0, 1,0,0,0, 0,0,0,0},
                            {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
                            {0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0},
                            {0,0,0,0, 0,0,0,0, 1,1,1,0, 0,0,0,0},
                            {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}};
    int FRAME_NUMBER = 0;
    SDL_Texture* spriteSheet;
    int SCREEN_WIDTH = 600;
    int SCREEN_HEIGHT = 600;
    void processInput();
    void updateGame();
    void generateOutput();
    void FillScreenWithGrass();
    bool loadFonts();
    void WallLayer();
    void Collectables();
    void DrawActor();
    bool get_intersection(int a_x, int a_y, int b_x, int b_y);
    bool handle_collisions(int x, int y);
    void handle_collectables();
    bool compute_collision(int player_x, int player_y);
    void displayFont();
    void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);


    bool mIsRunning;
    Inventory inventory;
    Player mPlayer;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;

    int mTicksCount;
    SDL_Texture* LoadTexture(const char* fileName);
};

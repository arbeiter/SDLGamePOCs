#include "game.h"
#include "textureManager.h"
#include "vector"
#include "tuple"
using namespace std;

int rect_height = 0;
int rect_width = 0;
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

Game::Game()
  :mWindow(nullptr)
  ,mRenderer(nullptr)
  ,mTicksCount(0)
  ,mPlayer(0,0, "")
{
}

int FRAME_NUMBER = 0;
SDL_Texture* spriteSheet;
int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;
//
// TODO: Move to class
std::vector<std::tuple<int, int>> brick_positions;
std::vector<std::tuple<int, int, int, int>> collectable_positions;

bool Game::initialize() {
  mPlayer.width = 50;
  mPlayer.height = 40;
  TextureManager tempTexManager = TextureManager(mRenderer);
  // Initialize SDL
  mIsRunning = true;
  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if (sdlResult != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  // Create an SDL Window
  mWindow = SDL_CreateWindow(
      "Pong Clone",
      100,	// Top left x-coordinate of window
      100,	// Top left y-coordinate of window
      1024,	// Width of window
      768,	// Height of window
      0		// Flags (0 for no flags set)
      );

  if (!mWindow)
  {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  //// Create SDL renderer
  mRenderer = SDL_CreateRenderer(
      mWindow, // Window to create renderer for
      -1,		 // Usually -1
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
      );

  if (!mRenderer)
  {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  return true;
}

void Game::runLoop() 
{
  TextureManager texManager = TextureManager(mRenderer);
  spriteSheet = texManager.preloadImage("adventurer-Sheet.png");
  while (mIsRunning)
  {
    processInput();
    updateGame();
    generateOutput();
  }
}

void Game::shutDown()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Game::processInput()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      // If we get an SDL_QUIT event, end loop
      case SDL_QUIT:
        mIsRunning = false;
        break;
    }

    // Get state of keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE])
    {
      mIsRunning = false;
    }

    bool do_not_move_temp = false;
    int player_x = mPlayer.x;
    int player_y = mPlayer.y;
    for(int i = 0; i < brick_positions.size(); i++) {
      int brick_x, brick_y;
      std::tie(brick_x, brick_y) = brick_positions[i];
      do_not_move_temp |= get_intersection(brick_x, brick_y, player_x, player_y);
    }

    int intersection_pos = -1;
    for(int i = 0; i < collectable_positions.size(); i++) {
      int collectable_x, collectable_y, a, b;
      std::tie(a, b, collectable_x, collectable_y) = collectable_positions[i];
      std::cout << collectable_x << collectable_y << std::endl;

      if(get_intersection(a, b, player_x, player_y)) {
        std::cout << "Intersection happened" << endl;
        mPlayer.x += 2;
        intersection_pos = i;
        std::cout << collectable_backing[collectable_x][collectable_y] << std::endl;
        collectable_backing[collectable_x][collectable_y] = 0;
        std::cout << "don't get here" << endl;
        break;
      }
    }

    if(intersection_pos != -1) {
      collectable_positions.erase(collectable_positions.begin() + intersection_pos);
    }

    // get rid of anything < 10
    // collectable_positions.erase(std::remove_if(collectable_positions.begin(), collectable_positions.end(), 
    //                        [](int i) { return i < 10; }), collectable_positions.end());

    if (state[SDL_SCANCODE_W])
    {
        if(do_not_move_temp == false)
          mPlayer.x += 1;
        else {
          mPlayer.x -= 1;
        }
    }

    if (state[SDL_SCANCODE_S])
    {
      if(mPlayer.x > 0) {
        if(do_not_move_temp == false)
          mPlayer.x -= 1;
        else {
          mPlayer.x += 1;
        }
      }
    }
    
    if (state[SDL_SCANCODE_A])
    {
      if(do_not_move_temp == false)
        mPlayer.y -= 1;
      else {
        mPlayer.y += 1;
      }
    }

    if (state[SDL_SCANCODE_D])
    {
      if(do_not_move_temp == false)
        mPlayer.y += 1;
      else {
        mPlayer.y -= 1;
      }
    }
  }
}

void Game::updateGame()
{
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

  if(deltaTime > 0.05f) {
    deltaTime = 0.05f;
  }

  mTicksCount = SDL_GetTicks();
  FRAME_NUMBER += 1;
}

void Game::generateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		0,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
  FillScreenWithGrass();
  Collectables();
  WallLayer();
  DrawActor();
  SDL_RenderPresent(mRenderer);
}

void Game::DrawActor() {
  TextureManager texManager = TextureManager(mRenderer);
  int num_frames = 8;
  int delayPerFrame = 500;
  int frame = (SDL_GetTicks() / delayPerFrame) % num_frames;
  mPlayer.sampleFromSpritesheet(&texManager,  spriteSheet, frame);
}


void Game::FillScreenWithGrass() {
  TextureManager texManager = TextureManager(mRenderer);
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/grass.png");

  int w, h;
  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);

  // 80, 18
  int x_w = w-2;
  int x_h = h;

  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    for (int j = 0; j <= SCREEN_WIDTH; j++) {
      // Wtf why are j and i reversed
      texManager.ClipTexture(bitmapTex, 0, 0, x_w, x_h, j, i, w+5, h+5);
      j+=w;
    }
    i+=h;
  }
}


void Game::Collectables() {
  TextureManager texManager = TextureManager(mRenderer);
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/collectable_jam.png");

  int w, h;
  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);

  int x_w = w;
  int x_h = h;
  rect_height = h;
  rect_width = w;

  int coord_x, coord_y;

  coord_x = 0;
  coord_y = 0;
  for (int i = 0; i < 11; i++) {
    for (int j = 0; j <16; j++) {
      if(collectable_backing[i][j] == 1) {
        texManager.ClipTexture(bitmapTex, 0, 0, x_w, x_h, coord_y, coord_x, w, h);
        collectable_positions.push_back(make_tuple(coord_y, coord_x));
      }
      coord_y += w;
    }
    coord_y = 0;
    coord_x += h;
  }
}

void Game::WallLayer() {
  int rr[11][16] = {{0,0,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,1,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,1,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}};
  TextureManager texManager = TextureManager(mRenderer);
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/wall.png");

  int w, h;

  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
  int x_w = w;
  int x_h = h;
  rect_height = h;
  rect_width = w;

  int coord_x, coord_y;

  coord_x = 0;
  coord_y = 0;
  for (int i = 0; i < 11; i++) {
    for (int j = 0; j <16; j++) {
      if(rr[i][j] == 1) {
        texManager.ClipTexture(bitmapTex, 0, 0, x_w, x_h, coord_y, coord_x, w, h);
        brick_positions.push_back(make_tuple(coord_y, coord_x));
      }
      coord_y += w;
    }
    coord_y = 0;
    coord_x += h;
  }
}

/*
bool Game::print_stats(int brick_x, int brick_y, int player_x, int player_y) {
  int player_width = mPlayer.width;
  int player_height = mPlayer.height;
  int brick_width = rect_width;
  int brick_height = rect_height;
  // std::cout << player_x << " " << player_y << std::endl;
  // std::cout << " " << b_x << " " << b_y << std::endl;
  // std::cout << a_width << " " << a_height << " " << b_width << " " << b_height<< std::endl;
}
*/

bool Game::get_intersection(int brick_x, int brick_y, int player_x, int player_y) {
  int player_width = mPlayer.width;
  int player_height = mPlayer.height;
  int brick_width = rect_width;
  int brick_height = rect_height;

  if(player_x + player_width < brick_x || player_x > brick_x + brick_width) {
    return false;
  }

  if(player_y > brick_y + brick_height ||  player_y + player_height < brick_y) {
    return false;
  }
  
  return true;
}

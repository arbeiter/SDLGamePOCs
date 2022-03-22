#include "game.h"
#include "textureManager.h"
#include "vector"
#include "tuple"
using namespace std;

int rect_height = 0;
int rect_width = 0;

Game::Game()
  :mWindow(nullptr)
  ,mRenderer(nullptr)
  ,mTicksCount(0)
  ,mPlayer(0,0, "")
{
}

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;
//
// TODO: Move to class
std::vector<std::tuple<int, int>> brick_positions;

bool Game::initialize() {
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
  mPlayer = Player(0, 300, "ground_grass_1.png");

  return true;
}

void Game::runLoop() 
{
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
      if(mPlayer.y > 0) {
        if(do_not_move_temp == false)
          mPlayer.y += 1;
        else {
          mPlayer.y -= 1;
        }
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
  WallLayer();
  DrawActor();
  SDL_RenderPresent(mRenderer);
}

void Game::DrawActor() {
  TextureManager texManager = TextureManager(mRenderer);
  mPlayer.draw(&texManager);
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

void Game::WallLayer() {
  int rr[11][16] = {{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, 
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

// a -> brick, b -> player
bool Game::get_intersection(int brick_x, int brick_y, int player_x, int player_y) {
  int player_width = mPlayer.width;
  int player_height = mPlayer.height;
  int brick_width = rect_width;
  int brick_height = rect_height;

  if(player_x + player_width < brick_x || player_x > brick_x + brick_width) {
    // std::cout << "X OUTSIDE" << std::endl;
    return false;
  }

  if(player_y > brick_y + brick_height ||  player_y + player_height < brick_y) {
    if(player_y > brick_y + brick_height) {
      // std::cout << "PLAYER_Y > BRICK_Y + BRICK_H" << endl;
    }
    if(player_y + player_height < brick_y) {
      // std::cout << "PLAYER_Y + PLAYER_H < BRICK_Y" << endl;
    }
    /*
    if(player_y > brick_y + brick_height) {
      std::cout << "PLAYER_Y > BRICK_Y + BRICK_H" << endl;
    }
    if(player_y + player_height < brick_y) {
      std::cout << "PLAYER_Y + PLAYER_H < BRICK_Y" << endl;
    }
    std::cout << "PLAYER XY "<< player_x << " " << player_y << "BRICKXY" << brick_x << " " << brick_y << std::endl;
    std::cout << "PLAYERWIDTH " << player_width << "PLAYERWIDTH" << player_height << "BRICKWIDTH " << brick_width << "BRICKHEIGHT " << brick_height << std::endl;
    std::cout << "BRICK HEIGHT SUM" << brick_y + brick_height << "PLAYER HEIGHT SUM" << player_y + player_height << std::endl;
    std::cout << "SUCKS Y" << endl;
    */
    return false;
  }
  
  // cout << "Intersection" << endl;
  /*
  std::cout << "PLAYER XY "<< player_x << " " << player_y << "BRICKXY" << brick_x << " " << brick_y << std::endl;
  std::cout << "PLAYERWIDTH " << player_width << "PLAYERWIDTH" << player_height << "BRICKWIDTH " << brick_width << "BRICKHEIGHT " << brick_height << std::endl;
  std::cout << "BRICK HEIGHT SUM" << brick_y + brick_height << "PLAYER HEIGHT SUM" << player_y + player_height << std::endl;
  */
  return true;
}

SDL_Texture* Game::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}

void Game::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(mRenderer, tex, &src, &dest, NULL, NULL, flip);
}

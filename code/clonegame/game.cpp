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

    if (state[SDL_SCANCODE_W])
    {
      int temp_x = mPlayer.x;
      int temp_y = mPlayer.y;
      bool do_not_move = false;
      for(int i = 0; i< brick_positions.size(); i++) {
        int curr_x, curr_y;
        std::tie(curr_x, curr_y) = brick_positions[i];
        do_not_move &= get_intersection(curr_x, curr_y, temp_x, temp_y);
        // cout << "CURR POS" << temp_x << " BRICK POS " << curr_x << endl;
      }
      if(do_not_move == false) {
        mPlayer.x += 1;
      }
    }
    if (state[SDL_SCANCODE_S])
    {
      if(mPlayer.x > 0) {
        mPlayer.x -= 1;
      }
    }
    if (state[SDL_SCANCODE_A])
    {
      mPlayer.y -= 1;
    }
    if (state[SDL_SCANCODE_D])
    {
      if(mPlayer.y > 0) {
        mPlayer.y += 1;
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
  // int num_cols = (int) SCREEN_WIDTH / w;
  // int num_rows = (int) SCREEN_HEIGHT / h;

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
  int rr[11][16] = {{1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}, 
                    {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1}};
  TextureManager texManager = TextureManager(mRenderer);
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/wall.png");

  int w, h;

  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
  int x_w = w;
  int x_h = h;
  rect_height = h;
  rect_width = w;

  int coord_x, coord_y;
  // std::cout << w << h << std::endl;

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

bool Game::get_intersection(int a_x, int a_y, int b_x, int b_y) {
  int b_width = mPlayer.width;
  int b_height = mPlayer.height;
  int a_width = rect_width;
  int a_height = rect_height;
  // check x intersection
  if (a_x + a_width < b_x || a_x > b_x + b_width) {
    if(a_y + a_height < b_y || a_y > b_y + b_height) {
      return false;
    }
  }
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

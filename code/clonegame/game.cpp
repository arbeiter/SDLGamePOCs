#include "game.h"
#include "textureManager.h"

Game::Game()
  :mWindow(nullptr)
  ,mRenderer(nullptr)
  ,mTicksCount(0)
{
}
int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;

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
  SDL_RenderPresent(mRenderer);
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
  int rr[4][4] = {{1,1,1,1}, {1,0,0,1}, {1,0,0,1}, {1,1,1,1}};
  TextureManager texManager = TextureManager(mRenderer);
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/wall.png");

  int w, h;

  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
  int x_w = w;
  int x_h = h;

  int coord_x, coord_y;
  std::cout << w << h << std::endl;

  coord_x = 0;
  coord_y = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j <4; j++) {
      if(rr[i][j] == 1) {
        texManager.ClipTexture(bitmapTex, 0, 0, x_w, x_h, coord_x, coord_y, w, h);
        std::cout << i << j << std::endl;
      }
      coord_y += w;
    }
    coord_y = 0;
    coord_x += h;
  }
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

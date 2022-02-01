#include "game.h"
#include "textureManager.h"

Game::Game()
  :mWindow(nullptr)
  ,mRenderer(nullptr)
  ,mTicksCount(0)
{
}

bool Game::initialize() {

  direction = 1;
  frameNumber = 0;
  position = 0.0f;
  PlayerX = 0;
  PlayerY = 0;
  player_vel.x = 10.0f;
  player_vel.y = 10.0f;

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
  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

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
  IMG_Quit();
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
      direction = -1;
    }
    if (state[SDL_SCANCODE_S])
    {
      direction = 1;
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

  frameNumber += 1;
  position += deltaTime;

  PlayerX += direction * player_vel.x * deltaTime;
  PlayerY += direction * player_vel.y * deltaTime;

  if(frameNumber > 3) {
    frameNumber = 0;
  }
  if(position > 1000) {
    position = 0;
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

  TextureManager texManager = TextureManager(mRenderer);
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/background.jpg");
  texManager.RenderTexture(bitmapTex, 0, 0);
  moveCharacter();

  SDL_RenderPresent(mRenderer);
}

void Game::moveCharacter() {
  TextureManager texManager = TextureManager(mRenderer);
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/tile.png");

  float clip_x;
  float clip_y;
  int clip_w, clip_h; 
  int player_x, player_y, player_w, player_h;

  clip_h = 48; 
  clip_w = 48;

  clip_x = (frameNumber + 1)* clip_w;  // src_w + x
  clip_y = 0; 
  
  player_x = PlayerX;
  player_y = PlayerY;
  player_w = 96;
  player_h = 96;

  texManager.ClipTexture(bitmapTex, clip_x, clip_y, clip_h, clip_w, player_x, player_y, player_h, player_w);
}

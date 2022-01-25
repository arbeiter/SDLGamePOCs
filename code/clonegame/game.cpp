#include "game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
  :mWindow(nullptr)
  ,mRenderer(nullptr)
  ,mTicksCount(0)
{
}

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

  mPaddlePos.x = 10.0f;
  mPaddlePos.y = 768.0f/2.0f;
  mBallPos.x = 1024.0f/2.0f;
  mBallPos.y = 768.0f/2.0f;
  mBallVel.x = -200.0f;
  mBallVel.y = 235.0f;
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
  mPaddleDir = 0;
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
      mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
      mPaddleDir += 1;
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
  if (mPaddleDir != 0)
  {
    mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
    if(mPaddlePos.y < (paddleH/2.0f + thickness)) {
      mPaddlePos.y = paddleH/2.0f + thickness;
    }
    else if (mPaddlePos.y > (768.0f - paddleH/2.0f - thickness)) 
    {
      mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
    }
  }
  mBallPos.x += mBallVel.x * deltaTime;
  mBallPos.y  += mBallVel.y * deltaTime;

  float diff = mPaddlePos.y - mBallPos.y;
  diff = (diff > 0.0f) ? diff : -diff;

  if(diff <= paddleH / 2.0f && 
     mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
     mBallVel.x < 0.0f) 
  {
    mBallVel.x *= -1.0f;
  }
  else if(mBallPos.x <= 0.0f)
  {
    mIsRunning = false;
  }
  else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
  {
    mBallVel.x *= -1.0f;
  }
  
  // top wall
  if(mBallPos.y <= thickness && mBallVel.y < 0.0f)
  {
    mBallVel.y *= -1;
  } 
  // Did the ball collide with the bottom wall?
  else if(mBallPos.y >= (768 - thickness) &&
          mBallVel.y > 0.0f)
  {
    mBallVel.y *= -1;
  }
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

	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
  SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw right wall
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

  SDL_Rect ball{
    static_cast<int>(mBallPos.x - thickness/2),
    static_cast<int>(mBallPos.y  - thickness/2),
    thickness,
    thickness
  };
	SDL_RenderFillRect(mRenderer, &ball);

  SDL_Rect paddle{
    static_cast<int>(mPaddlePos.x),
    static_cast<int>(mPaddlePos.y - paddleH/2),
    thickness,
    static_cast<int>(paddleH)
  };
	SDL_RenderFillRect(mRenderer, &paddle);

  SDL_RenderPresent(mRenderer);
}

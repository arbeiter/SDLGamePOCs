#include "game.h"
#include "textureManager.h"
#include "vector"
#include "tuple"
using namespace std;
//TODO 1: Render inventory.vector using inventory.render in draw()

Game::Game()
  :mWindow(nullptr)
  ,mRenderer(nullptr)
  ,inventory()
  ,mTicksCount(0)
  ,mPlayer(0,0, "")
{
}

// rgb(245,157,0)

// TODO: Move to class
std::vector<std::tuple<int, int, int, int>> collectable_positions;

bool Game::initialize() {
  // TODO: Move to player constructor
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

  // initialize SDL_ttf
  if (TTF_Init() == -1)
  {
    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    return false;
  }

  loadFonts();
  inventory = Inventory(mRenderer, mFont);
  return true;
}

bool Game::loadFonts() {
  mFont = TTF_OpenFont("./res/Inconsolata-Regular.ttf", 28);
  if (mFont == NULL)
  {
    printf("Failed to load font! SDL_ttf error: %s\n", TTF_GetError());
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
      case SDL_MOUSEMOTION:
        Uint32 buttons;
        buttons = SDL_GetMouseState(&m_mouseX,&m_mouseY);
        mouseState.sourceX = m_mouseX;
        mouseState.sourceY = m_mouseY;
        inventory.computeIntersection(m_mouseX, m_mouseY);
        break;
      case SDL_MOUSEBUTTONDOWN:
        switch(event.button.button) {
          case SDL_BUTTON_LEFT:
            SDL_GetMouseState(&m_mouseX,&m_mouseY);
            mouseState.sourceX = m_mouseX;
            mouseState.sourceY = m_mouseY;
            mouseState.dragMode = true;
            // check position in inventory squares
            printf("LEFT PRESSED %d %d\n", m_mouseX, m_mouseY);
            break;
          case SDL_BUTTON_RIGHT:
            printf("RIGHT\n");
            break;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        switch(event.button.button) {
          case SDL_BUTTON_LEFT:
            SDL_GetMouseState(&m_mouseX,&m_mouseY);
            mouseState.destinationX = m_mouseX;
            mouseState.destinationY = m_mouseY;
            printf("LEFT RAISED %d %d\n", m_mouseX, m_mouseY);
            printf("LEFT RAISED %f %f\n", mouseState.destinationX, mouseState.destinationY);

            if(mouseState.dragMode == true) {
              // inventory.check_if_in_destination_slot
              mouseState.dragMode = false;
            }
            break;
        }
        break;
    }

    // Get state of keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE])
    {
      mIsRunning = false;
    }


    int new_x = mPlayer.x;
    int new_y = mPlayer.y;
    if (state[SDL_SCANCODE_W])
    {
      new_x += 1;
    }
    if (state[SDL_SCANCODE_S])
    {
      new_x = new_x - 1 >= 0 ? new_x -1 : 0;
    }
    if (state[SDL_SCANCODE_A])
    {
      new_y += 1;
    }
    if (state[SDL_SCANCODE_D])
    {
      new_y = new_y - 1 >= 0 ? new_y -1 : 0;
    }
    bool collides_with_walls = handle_collisions(new_x, new_y);
    if(collides_with_walls == false) {
      mPlayer.x = new_x;
      mPlayer.y = new_y;
    }
  }
  inventory.setMouseState(mouseState);
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

  //Collectables();
  WallLayer();
  DrawActor();
  displayFont();
  inventory.draw();
  inventory.drawDropLocations();
  SDL_RenderPresent(mRenderer);
}

void Game::displayFont() {
  SDL_Surface* text;
  string pos = to_string(mPlayer.x) + "," + to_string(mPlayer.y) + ":" + debug_string;

  SDL_Color color = {1, 0, 0};
  text = TTF_RenderText_Solid(mFont, pos.c_str(), color);
  if(!text) {
    cout << "Failed to render text" << TTF_GetError() << endl;
  }

  SDL_Texture* text_texture;
  text_texture = SDL_CreateTextureFromSurface(mRenderer, text);
  SDL_Rect dest = { 0, 0, text->w, text->h};
  SDL_RenderCopy(mRenderer, text_texture, NULL, &dest);
  SDL_FreeSurface(text);
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
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/grass_texture_seamless.jpeg");

  int w, h;
  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);

  // 80, 18
  int x_w = w;
  int x_h = h;
  int num_tiles_cols = (int)(SCREEN_WIDTH/w);
  int num_tiles_rows = (int)(SCREEN_HEIGHT/h);

  for (int i = 0; i < 20; i+=1) {
    for (int j = 0; j < 20; j+=1) {
      // Wtf why are j and i reversed
      texManager.RenderTexture(bitmapTex, i * w, j * h, w, h);
    }
  }
}


void Game::handle_collectables() {
  /*
    int intersection_pos = -1;
    for(int i = 0; i < collectable_positions.size(); i++) {
      int collectable_i, collectable_j, a, b;
      std::tie(a, b, collectable_i, collectable_j) = collectable_positions[i];

      if(get_intersection(a, b, player_x, player_y)) {
        mPlayer.x += 2;
        intersection_pos = i;
        collectable_backing[collectable_i][collectable_j] = 0;
        break;
      }
    }

    if(intersection_pos != -1) {
      collectable_positions.erase(collectable_positions.begin() + intersection_pos);
    }
    */
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
        collectable_positions.push_back(make_tuple(coord_y, coord_x, i, j));
      }
      coord_y += w;
    }
    coord_y = 0;
    coord_x += h;
  }
}

void Game::WallLayer() {
  // TODO: Resize the texture so that blocks fill up all available space
  TextureManager texManager = TextureManager(mRenderer);
  SDL_Texture *bitmapTex = texManager.LoadTexture("./res/wall.png");

  int w, h;

  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
  int x_w = w;
  int x_h = h;

  int coord_x, coord_y;

  coord_x = 0;
  coord_y = 0;
  for (int i = 0; i < 11; i++) {
    for (int j = 0; j <16; j++) {
      if(rr[i][j] == 1) {
        texManager.ClipTexture(bitmapTex, 0, 0, x_w, x_h, coord_y, coord_x, w, h);
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

bool Game::handle_collisions(int x, int y) {
    bool do_not_move_temp = false;
    int player_x = x;
    int player_y = y;

    // screen_width, screen_height
      // num_walls_r = 11, num_walls_c = 16
      // dest_x = x +- width/2, see if there is a box there
      // dest_y = y +- height/2, see if there is a box there
      // check if dest_x, dest_y lies within a wall
      // dest_pos_x = dest_x / (16 * tilewidth);
      // dest_pos_y = dest_y / (11 * tileheight);
    // brute force collision
    // float player_left_bound = player_x - player_width / 2;
    // float player_right_bound = player_x + player_width / 2;

    int player_width = mPlayer.width * 2;
    int player_height = mPlayer.height * 2;

    int player_left_bound = player_x;
    int player_right_bound = player_x + player_width;
    int player_top_bound = player_y;
    int player_bottom_bound = player_y + player_height;

    bool collision = false;
    collision |= compute_collision(player_left_bound, player_top_bound);
    collision |= compute_collision(player_right_bound, player_top_bound);
    collision |= compute_collision(player_left_bound, player_bottom_bound);
    collision |= compute_collision(player_right_bound, player_bottom_bound);
    return collision;
}

bool Game::compute_collision(int player_x, int player_y) {
  int brick_width = 60;
  int brick_height = 60;
  int sbm_width = 60 * 11;
  int sbm_height = 60 * 16;

  int r_x = (int)( player_x / 60);
  int r_y = (int)( player_y / 60);

  if (rr[r_y][r_x] == 1) {
    return true;
  } else {
    return false;
  }
}

## ECS system

### Component

  Component(Actor owner, updateOrder)
  ~Component()
  Update(deltaTime)
  GetUpdateOrder { mUpdateOrder }

### Actor

  State
  Update(deltaTime)
  UpdateComponents(deltaTime)
  virtual UpdateActor(deltaTime)

  // Getters Position Scale Rotation State Game
  Game* mGame ,mComponents ,mPosition ,mScale ,mRotation ,mState

# Player <- (Actor)

  Player(Game * game)
  UpdateActor(deltaTime) override
  ProcessKeyboard(uint *state)
  GetRightSpeed
  GetDownSpeed

  private: mRightSpeed, mDownSpeed

# AnimSpriteComponent

   AnimSpriteComponent(Actor *owner, drawOrder)
   Update(float deltaTime) override
   SetAnimTextures(textures)
   GetAnimFPS, SetAnimFPS
    
   vector<SDL_Texture*> mAnimTextures
   mCurrFrame
   mAnimFPS

  
# BGSpriteComponent
    BGSpriteComponent(Actor *owner, drawOrder)
    Overriden:
      Update
      Draw
    SetBGTextures(vector<SDL_Textures*> &textures)
    SetScreenSize(size)

    vector<BGTexture> bgTextures, struct BGTexture(texture, offset), float scrollSpeeds

# SpriteComponent
    
    SpriteComponent(Actor * owner, drawOrder)
    ~SrpiteComponent()

    Draw(renderer)
    SetTexture(texture)

    mTexture, DrawOrder, TexWidth, TexHeight
# Game class details

## Game Init

1. LoadData:
  a. Create actor for player
    i. Set position and scale
  b. Create background textures using an actor
    i. set this actor's position to the middle of the screen
    ii. Create first background with draw order 100
      1. Set its scroll speed to -100
      2. Set its bg texture using Game.GetTexture
    ii. Create first background with draw order 100
      1. Set its scroll speed to -100
      2. Set its bg texture using Game.GetTexture

2. GetTexture: Load texture from file
   
## Game RunLoop 
1. RunLoop
  i. ProcessInput
    player->ProcessKeyboard(SDL input state)
  ii. UpdateGame
    a. mUpdatingActors = true 
      for(actor: mActors) actor.update(deltaTime)
      mUpdateActors = false
    b. Move all pending Actors back to mActors 
    c. Cull all dead actors and delete them
  iii. GenerateOutput
    RenderClear
    for sprite in Game.mSprites -> Draw all sprites

## Game class methods:
    AddSprite, RemoveSprite, AddActor, RemoveActor

# Implementation details

An actor owns a component
Sprites, BGSprites, AnimSprites all components

### Component
  Component(owner, updateOrder):mOwner(owner), mUPdateOrder(updateOrder) {
  mOwner->addComponent(this)
}

### Animation details

1. SpriteComponent:Draw uses RenderCopyEx using a
   rectangle

### Input processing






















# Chapter 3 Headers

### CircleComponent

GetRadius
SetRadius
GetCenter

Intersect(CircleComponent& a, CircleComponent& b)

### MoveComponent

Update: Overridden
AngularSpeed: Gettters/Setters
ForwardSpeed: Getters/Setters

### InputComponent <- MoveComponent

ProcessInput(keyState)

MaxForwardSpeed, MaxAngularSpeed
clockwiseKey, counterClockwiseKey, BackKey, ForwardKey

## Actors

### Actor class changes

virtual void ActorInput(keyState)
void ProcessInput(keyState)

### Laser
mDeathTimer
CircleComponent ``*circle``
void updateActor(deltaTime) override

### Asteroid
CircleComponent ``*circle``









































# Chapter 3 implementations

## Actors

### Asteroid
  Constructor:
    1. Initialize randPos to random vector
    2. set rotation to random range
    3. create a sprite component
    4. create a move component with forward speed 150.0f
    5. create a circle component for collision with a radius
    6. game->addAsteroid(this)

### Laser
  
  Constructor:
    1. Create a sprite component and set texture
    2. create a move component and set forward speed
    3. Create a circle component and set radius

  UpdateActor:
    1. Use delta time
    2. each time update is called, mDeathTimer is decremented by delta time
    3. when delta time == 0, the laser evaporates
    4. iterate over all asteroids:
      a. Compute intersection with asteriod circle using intersect
      b. if asteroid intersection happens, mark asteroid as dead







# Resources

thatgamesguy.uk
TileMaps:
https://github.com/connorkuehl/sdl2-tiled-example/blob/master/src/level.hpp








































































































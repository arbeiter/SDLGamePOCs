### One possible design

AssetManager
  AddTexture(id, path)
  GetTexture(id)
  AddFont(id, path, size, color)
  GetFont(id)
  map<string, SDL_Texture*> texture
  map<string, TTF_Font*> texture
Game
  handleEvents(), update(), running(), render(), clean()
  static SDL_Rect camera, AssetManager* assets, SDL_Renderer *renderer, SDL_Event event, SDL_Window *window
TextureManager
  static LoadTexture, Draw
Map
  LoadMap, AddTile

Component
Entity
Manager

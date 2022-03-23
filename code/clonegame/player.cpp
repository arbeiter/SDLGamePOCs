#include "player.h"

void Player::draw(TextureManager *texManager) {
  std::string fileName = "./res/" + name;
  const char* image = fileName.c_str();
  // TODO: Don't load this per draw, load it in advance
  SDL_Texture *bitmapTex = texManager->LoadTexture(image);
  int w, h;
  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
  width = w;
  height = h;
  texManager->ClipTexture(bitmapTex, 0, 0, w, h, x, y, w, h);
}

void Player::sampleFromSpritesheet(TextureManager *texManager, SDL_Texture* bitmapTex, int pos) {
  int sheet_w, sheet_h;
  int num_characters_per_row, character_w, character_h;
  character_w = 50;
  character_h = 50;

  SDL_QueryTexture(bitmapTex, NULL, NULL, &sheet_w, &sheet_h);
  // std::cout << sheet_w << " " << sheet_h << " " << pos << std::endl;
  num_characters_per_row = (int)sheet_w/character_w;
  int character_row = (int)(pos / num_characters_per_row);
  int character_column = (int)(pos % num_characters_per_row);
  int pos_y = character_row * character_h;
  int pos_x = character_column * character_w;
  texManager->ClipTexture(bitmapTex, pos_x, pos_y, character_w, character_h, x, y, character_w*2, character_h*2);
}

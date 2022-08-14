#include "player.h"

void Player::sampleFromSpritesheet(TextureManager *texManager, SDL_Texture* bitmapTex, int pos) {
  int sheet_w, sheet_h;
  int num_characters_per_row, character_w, character_h;
  character_w = width;
  character_h = height;

  SDL_QueryTexture(bitmapTex, NULL, NULL, &sheet_w, &sheet_h);
  // std::cout << sheet_w << " " << sheet_h << " " << pos << std::endl;
  num_characters_per_row = (int)sheet_w/character_w;
  int character_row = (int)(pos / num_characters_per_row);
  int character_column = (int)(pos % num_characters_per_row);
  int pos_y = character_row * character_h;
  int pos_x = character_column * character_w;
  texManager->ClipTexture(bitmapTex, pos_x, pos_y, character_w, character_h, x, y, character_w * 2, character_h * 2);
}

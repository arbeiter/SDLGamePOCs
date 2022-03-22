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

void Player::loadImage(TextureManager *texManager, int pos) {
  // character_w, character_h
  // sheet_w, sheet_h
  // TODO
  // h/image_size, w/image_size
  // divide into equal parts
  // load into array of images
  /*
     const char* image = "./res/" + name;
     SDL_Texture *bitmapTex = texManager->LoadTexture(image);
     int w, h;
     SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
     int num_characters_per_row, character_row, character_column;
     num_characters_per_row = sheet_w/character_w
     character_row = (int)(pos / num_characters_per_row);
     character_column = (int)(pos % num_characters_per_row);
     int y = character_row * sheet_h;
     int x = character_column * sheet_w;
     
     
     const char* image = "./res/" + name;
     SDL_Texture *bitmapTex = texManager->LoadTexture(image);
     int w, h;
     SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
     texManager->ClipTexture(bitmapTex, 0, 0, w, h, x, y, w, h);
   */
}

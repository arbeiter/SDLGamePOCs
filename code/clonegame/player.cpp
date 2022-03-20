#include "player.h"

void Player::draw(TextureManager *texManager) {
  std::string fileName = "./res/" + name;
  const char* image = fileName.c_str();
  SDL_Texture *bitmapTex = texManager->LoadTexture(image);
  int w, h;
  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
  texManager->ClipTexture(bitmapTex, 0, 0, w, h, x, y, w, h);
}

void Player::loadImages(TextureManager *texManager) {
  // TODO
  // h/image_size, w/image_size
  // divide into equal parts
  // load into array of images
  /*
     const char* image = "./res/" + name;
     SDL_Texture *bitmapTex = texManager->LoadTexture(image);
     int w, h;
     SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
     for(int i = 0; i < w/image_size; i++) {
      for(int j = 0; j < h/image_size; j++) {
        
      }
    }
  int w, h;
  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
  texManager->ClipTexture(bitmapTex, 0, 0, w, h, x, y, w, h);
   */
}

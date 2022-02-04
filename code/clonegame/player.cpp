#include "player.h"

void Player::draw(TextureManager *texManager) {
  int pos_x = x;
  int pos_y = y;
  SDL_Texture *bitmapTex = texManager->LoadTexture("./res/actor.png");
  int w, h;
  SDL_QueryTexture(bitmapTex, NULL, NULL, &w, &h);
  texManager->ClipTexture(bitmapTex, 0, 0, w, h, x, y, w, h);
}

#include "inventory.h"

// TODO: RefreshDisplay doesn't update the size of the items
// TODO: An inventory should always a transparent overlay
// TODO: An asset manager that reads a list of textures
// TODO: Click and drag behavior, hook into SDL events inside the class

void Inventory::draw() {
  refreshDisplay();
  // an inventory is always centered on the screen
  drawChrome();

}

void Inventory::refreshDisplay() {
  SDL_DisplayMode dm;
  if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
      SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
      return;
  }
  width = dm.w;
  height = dm.h;
}

void Inventory::drawChrome() {
  SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
  SDL_Rect srcrect;
  srcrect.x = (int)(width/3);
  srcrect.y = (int)(height/3);
  srcrect.w = (int)(width/3);
  srcrect.h = (int)(height/3);
  SDL_SetRenderDrawColor(mRenderer, 104, 104, 104, 150);
  SDL_RenderFillRect(mRenderer, &srcrect);
  SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_NONE);
  drawItems((int)(width/3), (int)(height/3));

}

void Inventory::drawItems(int w, int h) {
  int start_x = w;
  int start_y = h;

  int width = 40;
  int abs_width = start_x;
  int abs_height= start_y;

  int num_items_per_row = (int)(abs_width / width);
  int num_items_per_col = (int)(abs_height / width);
  int max_x = start_x + num_items_per_row * width;
  int max_y = start_y + num_items_per_col* width;

  for(int i = 0; i < num_items; i++) {
    SDL_Rect srcrect;
    int active_row = i / num_items_per_row;
    int active_column = i % num_items_per_row;
    int start_pos_x = active_column * width;
    int start_pos_y = active_row * width;

    srcrect.x = (start_pos_x + w);
    srcrect.y = (start_pos_y + h);
    srcrect.w = width;
    srcrect.h = width;

    SDL_Rect inner_rect;
    inner_rect.x = srcrect.x + 3;
    inner_rect.y = srcrect.y + 3;
    inner_rect.w = width - 6;
    inner_rect.h = width - 6;

    SDL_SetRenderDrawColor(mRenderer, 1, 1, 1, 1);
    SDL_RenderFillRect(mRenderer, &inner_rect);
    texManager->ClipTexture(bitmapTex, 0, 0, tex_w, tex_h, inner_rect.x + 1, inner_rect.y + 2, width - 8, width - 7);
  }
}

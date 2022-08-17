#include "inventory.h"

// TODO: RefreshDisplay doesn't update the size of the items
// TODO: Click and drag behavior, hook into SDL events inside the class
// TODO: Add orientation overlay: Top left or top right
// Move font rendering to a Text class
// Add item equipped tray

void Inventory::loadItemStats() {
  itemStats.clear();
  itemStats.push_back({3, "Bag"});
  itemStats.push_back({2, "Belt"});
  itemStats.push_back({10, "Helm"});
  itemStats.push_back({2, "Armor"});
  itemStats.push_back({1, "Iron Boot"});
  itemStats.push_back({4, "Iron Helment"});
}

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

  int width = 80;
  int abs_width = start_x;
  int abs_height= start_y;

  int num_items_per_row = (int)(abs_width / width);
  int num_items_per_col = (int)(abs_height / width);
  int max_x = start_x + num_items_per_row * width;
  int max_y = start_y + num_items_per_col* width;

  int num_items = itemStats.size();
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
    drawItem(srcrect, width, i);
  }
}

void Inventory::drawItem(SDL_Rect &srcrect, int width, int item_idx) {
    SDL_Rect inner_rect;
    inner_rect.x = srcrect.x + 3;
    inner_rect.y = srcrect.y + 3;
    inner_rect.w = width - 6;
    inner_rect.h = width - 6;

    SDL_SetRenderDrawColor(mRenderer, 1, 1, 1, 1);
    SDL_RenderFillRect(mRenderer, &inner_rect);

    // given the item's index
    // item height = (int)(tex_h / num_items)
    // item width = (int)(tex_w)
    int num_items = itemStats.size();
    int abs_w = tex_w;
    int abs_h = 32;
    int tex_offset_x = 0;
    int tex_offset_y = item_idx * abs_h;

    texManager->ClipTexture(bitmapTex,
                            tex_offset_x, tex_offset_y, abs_w, abs_h,
                            inner_rect.x + 1, inner_rect.y + 2, width - 8, width - 7);
    drawCount(srcrect, itemStats[item_idx].itemCount);
}

void Inventory::drawCount(SDL_Rect &srcrect, int count) {
  int num_segments = 6;
  int width = srcrect.w / num_segments;
  int height = srcrect.h / num_segments;
  int start_pos_x = srcrect.x + (num_segments - 1) * width;
  int start_pos_y = srcrect.y + (num_segments - 1) * width;
  SDL_Rect dest = { start_pos_x, start_pos_y, width, height};

  if(count == 1) {
    return;
  }

  SDL_Surface* text;
  string pos = to_string(count);

  SDL_Color color = {255, 255, 255};
  text = TTF_RenderText_Solid(font, pos.c_str(), color);
  if(!text) {
    cout << "Failed to render text" << TTF_GetError() << endl;
  }

  SDL_Texture* text_texture;
  text_texture = SDL_CreateTextureFromSurface(mRenderer, text);
  SDL_RenderCopy(mRenderer, text_texture, NULL, &dest);
  SDL_FreeSurface(text);
}
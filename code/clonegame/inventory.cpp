#include "inventory.h"

// TODO: Once drag is on, start tracking the mouse to draw the texture
  // Part I: Draw a transparent structure and make it track the mouse
    // Render the grid to a texture
    // Set blending on and set transparency to 40%, background to red
    // (X, Y) co-ordinates are centered at the current mouse location

  // Part II: Make the transparent structure overlap the destination grid position
    // If the destination (X, Y) co-ordinates are on the map
      // Validate the destination inside a region
    // If the destination (X, Y) co-ordinates are within a crafting tray
      // Snap item to fit
      // Find the items in the stats tuple and decrement values there

// TODO: Load item stats from a file instead
// TODO: RefreshDisplay doesn't update the size of the items, add SDL window resize callback
// TODO: Click and drag behavior, hook into SDL events inside the class
// TODO: Add orientation overlay: Top left or top right

// Move font rendering to a Text class
// Add item equipped tray

// TODO: Use STB_Img to blur the background or some image processing shiz instead of
// having to do that in gimp
// TODO: Use STB_Rectpack to pack sprites instead of having to do it with gimp

void Inventory::loadItemStats() {
  itemStats.clear();
  itemStats.push_back({3, "Bag", 0, 0, 0, 0});
  itemStats.push_back({2, "Belt", 0, 0, 0, 0});
  itemStats.push_back({10, "Helm", 0, 0, 0, 0});
  itemStats.push_back({2, "Armor", 0, 0, 0, 0});
  itemStats.push_back({1, "Iron Boot", 0, 0, 0, 0});
  itemStats.push_back({4, "Iron Helment", 0, 0, 0, 0});
}

void Inventory::draw() {
  refreshDisplay();
  // an inventory is always centered on the screen
  drawChrome();
  drawItems((int)(width/3), (int)(height/3));
  itemDraggable();
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

MouseState Inventory::setMouseState(MouseState mState) {
  // previous mouseState.
  int prevIndex = mouseState.selectedIndex;
  mouseState = mState;
  //printf("Setting mouse state %d %d \n", mouseState.sourceX, mouseState.sourceY);
  mouseState.selectedIndex = prevIndex;
  return mouseState;
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

int Inventory::computeIntersection(int x, int y) {
  // Brute force, could be optimized but who cares with < 1000 items
  int tlx = itemStats[0].top_left_x;
  int tly = itemStats[0].top_left_y;
  int tr_x = itemStats[0].top_left_x + itemStats[0].width;
  int tr_y = itemStats[0].top_left_y + itemStats[0].height;

  int intersect = -1;
  for(int i = 0; i < itemStats.size(); i++) {
    tlx = itemStats[i].top_left_x;
    tly = itemStats[i].top_left_y;
    tr_x = itemStats[i].top_left_x + itemStats[i].width;
    tr_y = itemStats[i].top_left_y + itemStats[i].height;
    bool x_inside = (x >= tlx && x <= tr_x);
    bool y_inside = (y >= tly && y <= tr_y);
    if(x_inside && y_inside) {
      intersect = i;
      break;
    }
  }
  highlighted_index = intersect;
  return intersect;
}

void Inventory::itemDraggable() {
  // if left click enabled and compute_intersection(x, y) == True
  // 1. Find original item position and print it
  // 2. Set item draggable to true and get mouse position
  // Set (x, y) position
  // Element not transparent
  if(mouseState.dragMode == true) {
    int intersect = computeIntersection(mouseState.sourceX, mouseState.sourceY);
    if(intersect != -1) {
      mouseState.selectedIndex = intersect;
      printf("Pressed and now selected Index set to 1 %d %d \n", mouseState.sourceX, mouseState.sourceY);
    }
  }

  if(mouseState.selectedIndex != -1 && mouseState.dragMode == false) {
    printf("Mouse state selected index after release %d\n", mouseState.selectedIndex);
    printf("Released and now selected Index set to 1 %f %f \n", mouseState.destinationX, mouseState.destinationY);
    printf("Released and now selected Index set to -1 \n");
    mouseState.selectedIndex = -1;
  }
}

void Inventory::drawItem(SDL_Rect &srcrect, int width, int item_idx) {
// DrawItem: DragItem enabled and check if intersection

    // Mouse Position
    //SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_ADD);
    SDL_Rect inner_rect;
    inner_rect.x = srcrect.x + 3;
    inner_rect.y = srcrect.y + 3;
    inner_rect.w = width - 6;
    inner_rect.h = width - 6;

    if(item_idx == highlighted_index) {
      SDL_SetRenderDrawColor(mRenderer, 245, 157, 0, 1);
    } else {
      SDL_SetRenderDrawColor(mRenderer, 1, 1, 1, 1);
    }
    SDL_RenderFillRect(mRenderer, &inner_rect);

    int num_items = itemStats.size();
    int abs_w = tex_w;
    int abs_h = 32;
    int tex_offset_x = 0;
    int tex_offset_y = item_idx * abs_h;

    int final_x = inner_rect.x + 1;
    int final_y = inner_rect.y + 2;
    int final_width = width - 8;
    int final_height = width - 7;
    texManager->ClipTexture(bitmapTex,
                            tex_offset_x, tex_offset_y,
                            abs_w, abs_h,
                            final_x, final_y,
                            final_width, final_height);

    drawCount(srcrect, itemStats[item_idx].itemCount);
    itemStats[item_idx].top_left_x = final_x;
    itemStats[item_idx].top_left_y = final_y;
    itemStats[item_idx].width = final_width;
    itemStats[item_idx].height = final_height;
    //SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_NONE);
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

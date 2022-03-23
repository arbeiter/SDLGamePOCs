#include "textureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
  if (tempSurface == NULL) {
    std::cout << "Load file name " << texture << std::endl;
    std::cout << "Failed to load image surface" << IMG_GetError();
  }
	SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	
	return tex;
}

void TextureManager::RenderTexture(SDL_Texture *tex, int x, int y, int w, int h) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(mRenderer, tex, NULL, &dst);
}

void TextureManager::ClipTexture(SDL_Texture *tex, float x, float y, int w, int h, int p_x, int p_y, int p_w, int p_h) {
	SDL_Rect src;
  src.x = x; 
  src.y = y;
  src.w = w;
  src.h = h;

	SDL_Rect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = p_w;
	dst.h = p_h;
	SDL_RenderCopy(mRenderer, tex, &src, &dst);
}

void TextureManager::RenderTexture(SDL_Texture *tex, int x, int y){
  int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	RenderTexture(tex, x, y, w, h);
}

SDL_Texture* TextureManager::preloadImage(std::string name) {
  std::string fileName = "./res/" + name;
  const char* image = fileName.c_str();
  SDL_Texture *bitmapTex = LoadTexture(image);
  int sheet_w, sheet_h;
  SDL_QueryTexture(bitmapTex, NULL, NULL, &sheet_w, &sheet_h);
  std::cout << "WHY" << sheet_w << " " << sheet_h << std::endl;
  return bitmapTex;
}

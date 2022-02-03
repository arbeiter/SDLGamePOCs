#include "textureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
  if (tempSurface == NULL) {
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
  int tex_w, tex_h;
	SDL_QueryTexture(tex, NULL, NULL, &tex_w, &tex_h);
  src.x = x; 
  src.y = y;
  src.w = tex_w;
  src.h = tex_h;

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

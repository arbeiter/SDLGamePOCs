#include "textureManager.h"

SDL_Texture* LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	
	return tex;
}

void Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(mRenderer, tex, &src, &dest, NULL, NULL, flip);
}

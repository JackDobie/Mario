#include "Texture2D.h"
#include <SDL_image.h>
#include <iostream>
using namespace::std;

SDL_Renderer* mRenderer;
SDL_Texture* mTexture;

int mWidth;
int mHeight;

Texture2D::Texture2D(SDL_Renderer* renderer)
{
	mRenderer = renderer;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Texture2D::~Texture2D()
{
	Free();
	mRenderer = NULL;
}

bool Texture2D::LoadFromFile(string path)
{
	Free();
	mTexture = NULL;

	SDL_Surface* pSurface = IMG_Load(path.c_str());
	if (pSurface != NULL)
	{
		mTexture = SDL_CreateTextureFromSurface(mRenderer, pSurface);
		mWidth = pSurface->w;
		mHeight = pSurface->h;
	}
	else if (pSurface == NULL)
	{
		cout << "Unable to create texture from surface. Error: " << IMG_GetError() << endl;
		return mTexture != NULL;
	}

	SDL_FreeSurface(pSurface);
	return mTexture != NULL;
}

void Texture2D::Free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture2D::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle)
{
	SDL_Rect renderLocation = { newPosition.x, newPosition.y, mWidth, mHeight }; //where to render
	SDL_RenderCopyEx(mRenderer, mTexture, NULL, &renderLocation, 0, NULL, flip); //render to screen
}

void Texture2D::Render(SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle)
{
	SDL_RenderCopyEx(mRenderer, mTexture, &srcRect, &destRect, angle, NULL, flip); //render to screen
}
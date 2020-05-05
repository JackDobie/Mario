#pragma once

#ifndef _POWBLOCK_H
#include "SDL.h"
#include "Commons.h"
#include "Texture2D.h"
#include "LevelMap.h"
#include <iostream>

class PowBlock
{
public:
	PowBlock();
	PowBlock(SDL_Renderer* renderer, LevelMap* map);
	~PowBlock();

	void	Render();
	Rect2D	GetCollisionBox()
	{
		return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth() / 3, mTexture->GetHeight());
	};
	void TakeAHit();
	bool	IsAvailable()
	{
		return mNumberofHitsLeft > 0;
	};
private:
	Vector2D		mPosition;
	Texture2D*		mTexture;
	LevelMap*		mLevelMap;

	float	mSingleSpriteWidth;
	float	mSingleSpriteHeight;

	int		mNumberofHitsLeft;
};

#endif // !_POWBLOCK_H
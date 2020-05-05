#include "PowBlock.h"

PowBlock::PowBlock(SDL_Renderer* renderer, LevelMap* map)
{
	string imagePath = "Images/PowBlock.png";
	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(imagePath))
	{
		cout << "Failed to load texture: " << imagePath << endl;
		return;
	}

	mLevelMap			= map;

	mSingleSpriteWidth	= mTexture->GetWidth() / 3; //powblock spritesheet has three sprites in one row
	mSingleSpriteHeight	= mTexture->GetHeight();
	mNumberofHitsLeft	= 3;
	mPosition			= Vector2D((SCREEN_WIDTH * 0.5f) - mSingleSpriteWidth * 0.5f, 260);
}
PowBlock::~PowBlock()
{
	mTexture = NULL;
	delete mTexture;
	mLevelMap = NULL;
	delete mLevelMap;
}

void PowBlock::TakeAHit()
{
	mNumberofHitsLeft--;
	if (mNumberofHitsLeft <= 0)
	{
		mNumberofHitsLeft = 0;
		mLevelMap->ChangeTileAt(8, 7, 0);
		mLevelMap->ChangeTileAt(8, 8, 0);
	}
}

void PowBlock::Render()
{
	if (mNumberofHitsLeft > 0)
	{
		//get the portion of the spritesheet to draw
		int left = mSingleSpriteWidth * (mNumberofHitsLeft - 1);

		//								{ XPos, YPos, WidthOfSingleSprite, HeightOfSingleSprite }
		SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

		//determine where it draws
		SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

		//draw
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE, 0.0f);
	}
}
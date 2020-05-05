#include "EnemyKoopa.h"

EnemyKoopa::EnemyKoopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING startFacing, float movementSpeed, SoundEffect* sounds) : Character(renderer, imagePath, startPosition, map)
{
	mRenderer			= renderer;
	mTexture			= new Texture2D(mRenderer);
	mTexture			->LoadFromFile(imagePath);
	mFacingDirection	= startFacing;
	mMovementSpeed		= movementSpeed;
	mPosition			= startPosition;
	mInjured			= false;
	mInjuredTime		= INJURED_TIME;
	isAlive				= true;
	isDying				= false;
	hasBeenHit			= false;
	mCanJump			= true;
	mJumpForce			= INITIAL_JUMP_FORCE_SMALL;
	mJumping			= false;
	mSingleSpriteWidth	= mTexture->GetWidth() / 2; //two sprites in one row
	mSingleSpriteHeight	= mTexture->GetHeight();
	mCollisionRadius	= 10.0f;
	mCurrentLevelMap	= map;
}
EnemyKoopa::~EnemyKoopa()
{
	mRenderer = NULL;
	mTexture = NULL;
	delete mTexture;
	mRenderer = NULL;
	delete mRenderer;
	mCurrentLevelMap = NULL;
	delete mCurrentLevelMap;
}

void EnemyKoopa::TakeDamage()
{
	mInjured = true;
	mInjuredTime = INJURED_TIME;
	Jump();
}

void EnemyKoopa::Jump()
{
	if (!mJumping)
	{
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;

		mJumping = true;
		mCanJump = false;
	}
}

void EnemyKoopa::FlipRightWayUp()
{
	Turn();
	mInjured = false;
	Jump();
}

void EnemyKoopa::Render()
{
	//variable to hold the left position of the sprite to draw
	int left = 0.0f;
	//if injured move the left pos to be the left pos of the second image on the spritesheet
	if (mInjured) left = mSingleSpriteWidth;
	//get the portion of the spritesheet to draw
	//								{XPos,YPos,WidthOfSingleSprite,HeightOfSingleSprite}
	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };
	//where to draw
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };
	//draw
	mTexture->Render(portionOfSpritesheet, destRect, mFacingDirection == FACING_RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL, 0.0f);
}

void EnemyKoopa::Update(float deltaTime, SDL_Event e)
{
	//cout << "koopa: " << mPosition.x << ", " << mPosition.y << endl;
	if (mJumping)
	{
		mPosition.y -= mJumpForce * deltaTime;
		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;
		if (mJumpForce <= 0.0f)
		{
			mJumpForce = 0.0f;
			mJumping = false;
		}
	}

	if (!isDying)
	{
		if (mPosition.x < 0)
			mPosition.x = SCREEN_WIDTH;
		else if (mPosition.x > SCREEN_WIDTH)
			mPosition.x = 0;

		int centralXPosition	= (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
		int leftXPosition		= (int)(mPosition.x) / TILE_WIDTH;
		int footPosition		= (int)(mPosition.y + mTexture->GetHeight()) / TILE_HEIGHT;

		if (mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) == 0 &&
			mCurrentLevelMap->GetTileAt(footPosition, leftXPosition) == 0 && !mJumping) //rightpos breaks it but works fine without
		{
			Gravity(deltaTime);
		}
		else
			mCanJump = true;

		if (!mInjured)
		{
			//move when not injured
			if (mFacingDirection == FACING_LEFT)
			{
				mMovingLeft = true;
				mMovingRight = false;
			}
			else if (mFacingDirection == FACING_RIGHT)
			{
				mMovingRight = true;
				mMovingLeft = false;
			}

			if (mPosition.y > 345)
			{
				if (mPosition.x < 20)
				{
					//move to top left
					mPosition.x = 20.0f;
					mPosition.y = 32.0f;
					mFacingDirection = FACING_RIGHT;
				}
				else if (mPosition.x > 455)
				{
					//move to top right
					mPosition.x = 455.0f;
					mPosition.y = 32.0f;
					mFacingDirection = FACING_LEFT;
				}
			}
		}
		else
		{
			//do not move when injured
			mMovingLeft = false;
			mMovingRight = false;
			//count down to the injured time
			mInjuredTime -= deltaTime;

			if (mInjuredTime <= 0.0f) FlipRightWayUp();
		}
	}
	else
	{
		Gravity(deltaTime);//fall through map
	}

	if (mPosition.y > SCREEN_HEIGHT)
		isAlive = false;

	if (mMovingLeft && !mMovingRight)
		MoveLeft(deltaTime);
	else if (mMovingRight && !mMovingLeft)
		MoveRight(deltaTime);
}

void EnemyKoopa::MoveLeft(float deltaTime)
{
	mPosition.x -= mMovementSpeed * deltaTime;
}
void EnemyKoopa::MoveRight(float deltaTime)
{
	mPosition.x += mMovementSpeed * deltaTime;
}

Rect2D EnemyKoopa::GetCollisionBox()
{
	return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth(), mTexture->GetHeight());
}

void EnemyKoopa::Gravity(float deltaTime)
{
	mPosition.y += GRAVITY * deltaTime;
}

void EnemyKoopa::Turn()
{
	switch (mFacingDirection)
	{
	case FACING_LEFT:
		mFacingDirection = FACING_RIGHT;
		//mPosition.x += (mTexture->GetWidth() * 2) / TILE_WIDTH;
		break;
	case FACING_RIGHT:
		mFacingDirection = FACING_LEFT;
		//mPosition.x -= (mTexture->GetWidth() * 2) / TILE_WIDTH;
		break;
	}
}

void EnemyKoopa::CheckIfHit(int h, int w)
{
	int centralXPosition = (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
	int headPosition = (int)(mPosition.y + 10) / TILE_HEIGHT;

	if (headPosition == h && centralXPosition == w)
	{
		TakeDamage();
	}
}

bool EnemyKoopa::IsInPipe()
{
	if ((mPosition.y > SCREEN_HEIGHT || mPosition.y <= 75.0f) && (mPosition.x < 75.0f || mPosition.x > 390.0f))
	{
		return true;
	}
	else
		return false;
}

bool EnemyKoopa::GetInjured()
{
	return mInjured;
}
bool EnemyKoopa::GetAlive()
{
	return isAlive;
}
void EnemyKoopa::Kill()
{
	Jump();
	hasBeenHit = true;
	isDying = true;
}
bool EnemyKoopa::HasBeenHit()
{
	return hasBeenHit;
}

Vector2D EnemyKoopa::GetCentralPosition()
{
	return Vector2D((int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH, (int)(mPosition.y + 10) / TILE_HEIGHT);
}
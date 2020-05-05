#include "EnemyCrab.h"

EnemyCrab::EnemyCrab(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING startFacing, float movementSpeed, SoundEffect* sounds) : Character(renderer, imagePath, startPosition, map)
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
	hitCount			= 0;
	mCanJump			= true;
	mJumpForce			= INITIAL_JUMP_FORCE_SMALL;
	mJumping			= false;
	mSingleSpriteWidth	= mTexture->GetWidth() / 2; //two sprites in one row
	mSingleSpriteHeight	= mTexture->GetHeight();
	mCollisionRadius	= 10.0f;
	mCurrentLevelMap	= map;
	animCounter			= 0.0f;
}
EnemyCrab::~EnemyCrab()
{
	mRenderer = NULL;
	mTexture = NULL;
	delete mTexture;
	mRenderer = NULL;
	delete mRenderer;
	mCurrentLevelMap = NULL;
	delete mCurrentLevelMap;
}

void EnemyCrab::TakeDamage()
{
	if (++hitCount >= 2)
	{
		mInjured = true;
		mInjuredTime = INJURED_TIME;
	}
	else
		mMovementSpeed = CRAB_SPEED_ANGRY;
	Jump();
}

void EnemyCrab::Jump()
{
	if (!mJumping)
	{
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;
		mJumping = true;
		mCanJump = false;
	}
}

void EnemyCrab::FlipRightWayUp()
{
	Turn();
	mInjured = false;
	hitCount = 0;
	mMovementSpeed = CRAB_SPEED_NORMAL;
	Jump();
}

void EnemyCrab::Render()
{
	//variable to hold the left position of the sprite to draw
	int left = 0.0f;

	//change sprite every half second for when not hit, and every quarter second when hit
	if (hitCount == 0)
	{
		if (animCounter < 0.5f)
			left = 0.0f;
		else if (animCounter > 0.5f)
			left = mSingleSpriteWidth;
	}
	else if (hitCount == 1)
	{
		if (animCounter < 0.25f)
			left = 0.0f;
		else if(animCounter > 0.25f)
			left = mSingleSpriteWidth;
	}

	//get the portion of the spritesheet to draw
	//								{XPos,YPos,WidthOfSingleSprite,HeightOfSingleSprite}
	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };
	//where to draw
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };
	//draw
	mTexture->Render(portionOfSpritesheet, destRect, mInjured ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE, 0.0f);
}

void EnemyCrab::Update(float deltaTime, SDL_Event e)
{
	if (!mInjured)
	{
		animCounter += deltaTime;

		if (hitCount == 0)
		{
			if (animCounter > 1.0f)
			{
				animCounter = 0.0f;
			}
		}
		else if (hitCount == 1)
		{
			if (animCounter > 0.5f)
			{
				animCounter = 0.0f;
			}
		}
	}

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

		int centralXPosition = (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
		int leftXPosition = (int)(mPosition.x) / (TILE_WIDTH);
		int footPosition = (int)(mPosition.y + mTexture->GetHeight()) / TILE_WIDTH;

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

			if (mInjuredTime <= 0.0f)
				FlipRightWayUp();
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

void EnemyCrab::MoveLeft(float deltaTime)
{
	mPosition.x -= mMovementSpeed * deltaTime;
}
void EnemyCrab::MoveRight(float deltaTime)
{
	mPosition.x += mMovementSpeed * deltaTime;
}

Rect2D EnemyCrab::GetCollisionBox()
{
	return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth() / 2, mTexture->GetHeight() / 2);
}

void EnemyCrab::Gravity(float deltaTime)
{
	mPosition.y += GRAVITY * deltaTime;
}

void EnemyCrab::Turn()
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

void EnemyCrab::CheckIfHit(int h, int w)
{
	int centralXPosition = (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
	int headPosition = (int)(mPosition.y + 10) / TILE_HEIGHT;

	if (headPosition == h && centralXPosition == w)
	{
		TakeDamage();
	}
}

bool EnemyCrab::IsInPipe()
{
	if ((mPosition.y > SCREEN_HEIGHT || mPosition.y <= 75.0f) && (mPosition.x < 75.0f || mPosition.x > 390.0f))
	{
		return true;
	}
	else
		return false;
}

bool EnemyCrab::GetInjured()
{
	return mInjured;
}
bool EnemyCrab::GetAlive()
{
	return isAlive;
}
void EnemyCrab::Kill()
{
	Jump();
	hasBeenHit = true;
	isDying = true;
}
bool EnemyCrab::HasBeenHit()
{
	return hasBeenHit;
}

Vector2D EnemyCrab::GetCentralPosition()
{
	return Vector2D((int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH, (int)(mPosition.y + 10) / TILE_HEIGHT);
}
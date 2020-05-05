#include "Character.h"

Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map)
{
	mRenderer = renderer;
	mTexture = new Texture2D(mRenderer);
	mTexture->LoadFromFile(imagePath);
	mPosition = startPosition;
	mFacingDirection = FACING_RIGHT;
	mMovingLeft = false;
	mMovingRight = false;
	mJumping = false;
	mCanJump = true;
	mCollisionRadius = 15.0f;
	mCurrentLevelMap = map;
}

Character::~Character()
{
	mRenderer = NULL;
}

void Character::Render()
{
	mTexture->Render(mPosition, (mFacingDirection == FACING_RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL), 0); //if not facing right, flip the sprite to face left
}
void Character::Update(float deltaTime, SDL_Event e)
{
	//deal with jumping before movement
	if (mJumping)
	{
		mPosition.y -= mJumpForce * deltaTime; //move up by jump force

		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime; //decrement the jump force over time

		if (mJumpForce <= 0.0f) //when jump force decremented to 0 or below, don't jump
			mJumping = false;
	}

	int centralXPosition = (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
	int footPosition = (int)(mPosition.y + mTexture->GetHeight()) / TILE_HEIGHT;
	int headPosition = (int)(mPosition.y + 10) / TILE_HEIGHT;

	//deal with gravity
	if (mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) == 0)
	{
		AddGravity(deltaTime);
	}
	else
	{
		//collided with ground so player can jump again
		mCanJump = true;
	}
	//head collision
	if (mCurrentLevelMap->GetTileAt(headPosition, centralXPosition) == 1)
	{
		CancelJump();
	}


	if (mMovingLeft)
	{
		MoveLeft(deltaTime);
	}
	else if (mMovingRight)
	{
		MoveRight(deltaTime);
	}
}

void Character::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

Vector2D Character::GetPosition()
{
	return mPosition;
}

Vector2D Character::GetCentralPosition()
{
	return Vector2D((int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH, (int)(mPosition.y + 10) / TILE_HEIGHT);
}

void Character::MoveLeft(float deltaTime)
{
	mFacingDirection = FACING_LEFT;
	mPosition.x -= MovementSpeed * deltaTime;
}
void Character::MoveRight(float deltaTime)
{
	mFacingDirection = FACING_RIGHT;
	mPosition.x += MovementSpeed * deltaTime;
}

void Character::AddGravity(float deltaTime)
{
	mPosition.y += GRAVITY * deltaTime;
	mCanJump = false;
}
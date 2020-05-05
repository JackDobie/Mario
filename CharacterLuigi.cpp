#include "CharacterLuigi.h"
#include "GameScreenLevel1.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map) : Character(renderer, imagePath, startPosition, map)
{
	mRenderer = renderer;
	mTexture = new Texture2D(mRenderer);
	mTexture->LoadFromFile(imagePath);
	mPosition = startPosition;
	mFacingDirection = FACING_LEFT;
	mMovingLeft = false;
	mMovingRight = false;
	mJumping = false;
	mCanJump = true;
	mCollisionRadius = 15.0f;
	mCurrentLevelMap = map;
	isAlive = true;
	livesLeft = PLAYER_LIVES_MAX;
	respawnPosition = Vector2D(400, 10);
	isRespawning = false;
	respawnTimer = 0.0f;
}

CharacterLuigi::~CharacterLuigi()
{
	mRenderer = NULL;
	mTexture = NULL;
	delete mTexture;
	mRenderer = NULL;
	delete mRenderer;
	mCurrentLevelMap = NULL;
	delete mCurrentLevelMap;
	delete& respawnPosition;
}

void CharacterLuigi::Render()
{
	mTexture->Render(mPosition, (mFacingDirection == FACING_RIGHT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE), 0); //luigi faces the other direction to mario, so flip the sprite
}

void CharacterLuigi::Update(float deltaTime, SDL_Event e)
{
	if (mJumping)
	{
		mPosition.y -= mJumpForce * deltaTime; //move up by jump force

		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime; //decrement the jump force over time

		if (mJumpForce <= 0.0f) //when jump force decremented to 0 or below, don't jump
		{
			mJumpForce = 0.0f;
			mJumping = false;
		}
	}
	if(isAlive && !isRespawning)
	{
		if (mPosition.x < 0)
			mPosition.x = SCREEN_WIDTH;
		else if (mPosition.x > SCREEN_WIDTH)
			mPosition.x = 0;

		int centralXPosition = (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
		int footPosition = (int)(mPosition.y + mTexture->GetHeight()) / TILE_HEIGHT;
		int headPosition = (int)(mPosition.y + 10) / TILE_HEIGHT;

		//deal with gravity
		if (mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) == 0 && mPosition.y < 342)
		{
			Gravity(deltaTime);
		}
		else
		{
			//collided with ground so player can jump again
			mCanJump = true;
		}
		//head collision
		if (mCurrentLevelMap->GetTileAt(headPosition, centralXPosition) == 1)
		{
			if (mCurrentLevelMap->GetTileAt(headPosition - 1, centralXPosition) == 0)//if tile above collision is 0
			{
				//call enemy void to check if pos in tile above, which causes them to fall over
				for (unsigned int i = 0; i < GameScreenLevel1::mEnemies.size(); i++)
				{
					if (GameScreenLevel1::mEnemies[i]->GetCentralPosition().x == centralXPosition &&
						GameScreenLevel1::mEnemies[i]->GetCentralPosition().y == headPosition - 1)
					{
						GameScreenLevel1::mEnemies[i]->TakeDamage();
					}
				}
			}
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

		SDL_PollEvent(&e); //get the events
		switch (e.type)
		{
		case SDL_KEYDOWN: //when pressed
			switch (e.key.keysym.sym)
			{
			case SDLK_a:
				mMovingLeft = true;
				break;
			case SDLK_d:
				mMovingRight = true;
				break;
			case SDLK_w:
				Jump();
				break;
			}
			break;
		case SDL_KEYUP: //when released
			switch (e.key.keysym.sym)
			{
			case SDLK_a:
				mMovingLeft = false;
				break;
			case SDLK_d:
				mMovingRight = false;
				break;
			}
			break;
		}
	}
	else if (!isAlive)
	{
		Gravity(deltaTime);
	}

	if (mPosition.y > SCREEN_HEIGHT)
	{
		if (livesLeft > 0)
		{
			livesLeft--;
			mPosition = respawnPosition;
			isAlive = true;
			isRespawning = true;
		}
		else
		{
			//luigi is kil
		}
	}
	if (isRespawning)
	{
		if (respawnTimer < 3.0f)
		{
			respawnTimer += deltaTime;
		}
		else
			isRespawning = false;
	}
}

void CharacterLuigi::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}
Vector2D CharacterLuigi::GetPosition()
{
	return mPosition;
}

void CharacterLuigi::MoveLeft(float deltaTime)
{
	mFacingDirection = FACING_LEFT;
	mPosition.x -= MovementSpeed * deltaTime;
}
void CharacterLuigi::MoveRight(float deltaTime)
{
	mFacingDirection = FACING_RIGHT;
	mPosition.x += MovementSpeed * deltaTime;
}

void CharacterLuigi::Gravity(float deltaTime)
{
	mPosition.y += GRAVITY * deltaTime;
	mCanJump = false;
}

Rect2D CharacterLuigi::GetCollisionBox()
{
	return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth(), mTexture->GetHeight());
}

bool CharacterLuigi::GetAlive()
{
	return isAlive;
}
int CharacterLuigi::GetLivesLeft()
{
	return livesLeft;
}

void CharacterLuigi::Kill()
{
	isAlive = false;
	Jump();
}
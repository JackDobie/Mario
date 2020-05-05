#include "CharacterMario.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "GameScreenManager.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map, SoundEffect* sounds) : Character(renderer, imagePath, startPosition, map)
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
	isAlive = true;
	livesLeft = PLAYER_LIVES_MAX;
	respawnPosition = Vector2D((SCREEN_WIDTH / 2) - (mTexture->GetWidth() / 2), 10);
	isRespawning = false;
	respawnTimer = 0.0f;
	mSounds = sounds;
	endOfGame = false;
}

CharacterMario::~CharacterMario()
{
	mRenderer = NULL;
	mTexture = NULL;
	delete mTexture;
	mRenderer = NULL;
	delete mRenderer;
	mCurrentLevelMap = NULL;
	delete mCurrentLevelMap;
}

void CharacterMario::Render()
{
	mTexture->Render(mPosition, (mFacingDirection == FACING_RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL), 0);
}

void CharacterMario::Update(float deltaTime, SDL_Event e)
{
	//cout << "mario: " << mPosition.x << ", " << mPosition.y << endl;
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

	SDL_PollEvent(&e); //get the events
	switch (e.type)
	{
	case SDL_KEYDOWN: //when pressed
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			mMovingLeft = true;
			break;
		case SDLK_RIGHT:
			mMovingRight = true;
			break;
		case SDLK_UP:
			if(isAlive && !isRespawning && !endOfGame) Jump();
			break;
		}
		break;
	case SDL_KEYUP: //when released
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			mMovingLeft = false;
			break;
		case SDLK_RIGHT:
			mMovingRight = false;
			break;
		}
		break;
	}

	if (isAlive && !isRespawning && !endOfGame)
	{
		if (mPosition.x < 0)
			mPosition.x = SCREEN_WIDTH;
		else if (mPosition.x > SCREEN_WIDTH)
			mPosition.x = 0;

		int centralXPosition	= (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
		int leftXPosition		= (int)(mPosition.x) / TILE_WIDTH;
		int leftXPosition2		= (int)(mPosition.x + 4) / TILE_WIDTH; //closer to the middle. for collision with head rather than edge of sprite
		int rightXPosition		= (int)(mPosition.x + mTexture->GetWidth()) / TILE_HEIGHT;
		int rightXPosition2		= (int)(mPosition.x + mTexture->GetWidth() - 4) / TILE_HEIGHT; //closer to the middle. for collision with head rather than edge of sprite
		int footPosition		= (int)(mPosition.y + mTexture->GetHeight()) / TILE_HEIGHT;
		int headPosition		= (int)(mPosition.y + 10) / TILE_HEIGHT;
		int midYPosition		= (int)(mPosition.y + (mTexture->GetHeight() * 0.9f)) / TILE_HEIGHT;

		//deal with gravity
		if (mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) != 1 &&
			mCurrentLevelMap->GetTileAt(footPosition, leftXPosition) != 1 &&
			mCurrentLevelMap->GetTileAt(footPosition, rightXPosition) != 1)
		{
			Gravity(deltaTime);
		}
		else
		{
			if (mCurrentLevelMap->GetTileAt(midYPosition, centralXPosition) == 1 ||
				mCurrentLevelMap->GetTileAt(midYPosition, leftXPosition2) == 1 ||
				mCurrentLevelMap->GetTileAt(midYPosition, rightXPosition2) == 1) //if walking inside a block
			{
				mPosition.y -= (mTexture->GetHeight() * 0.1f);
			}
			else
				mCanJump = true; //collided with ground so player can jump again
		}

		//head collision
		if ((mCurrentLevelMap->GetTileAt(headPosition, centralXPosition) == 1 ||
			mCurrentLevelMap->GetTileAt(headPosition, leftXPosition2) == 1 ||
			mCurrentLevelMap->GetTileAt(headPosition, rightXPosition2) == 1) && mJumpForce > 30)
		{
			if (mCurrentLevelMap->GetTileAt(headPosition - 1, centralXPosition) == 0)//if tile above collision is 0
			{
				//call enemy void to check if pos in tile above, which causes them to fall over
				if (GameScreenManager::GetCurrentScreen() == SCREEN_LEVEL1)
				{
					for (unsigned int i = 0; i < GameScreenLevel1::mEnemies.size(); i++)
					{
						if (GameScreenLevel1::mEnemies[i]->GetCentralPosition().x == centralXPosition &&
							GameScreenLevel1::mEnemies[i]->GetCentralPosition().y == headPosition - 1)
						{
							GameScreenLevel1::mEnemies[i]->TakeDamage();
						}
					}
				}
				else if (GameScreenManager::GetCurrentScreen() == SCREEN_LEVEL2)
				{
					for (unsigned int i = 0; i < GameScreenLevel2::mEnemies.size(); i++)
					{
						if (GameScreenLevel2::mEnemies[i]->GetCentralPosition().x == centralXPosition &&
							GameScreenLevel2::mEnemies[i]->GetCentralPosition().y == headPosition - 1)
						{
							GameScreenLevel2::mEnemies[i]->TakeDamage();
						}
					}
				}
			}
			CancelJump();
			mSounds->PlayToChannel(-1, "Music/Bump.wav", 0, false);
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
	else if (!isAlive && !isRespawning && !endOfGame)
	{
		Gravity(deltaTime);
	}

	if (mPosition.y > SCREEN_HEIGHT)
	{
		if (livesLeft > 0)
		{
			livesLeft--;
			isAlive = true;
			isRespawning = true;
			mPosition = respawnPosition;
		}
		else
		{
			endOfGame = true;
			//mario is kil
		}
	}
	if (isRespawning)
	{
		if (respawnTimer < 3.0f)
		{
			respawnTimer += deltaTime;
		}
		else if(respawnTimer > 3.0f || mMovingLeft || mMovingRight)
			isRespawning = false;
	}
}

void CharacterMario::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}
Vector2D CharacterMario::GetPosition()
{
	return mPosition;
}

void CharacterMario::MoveLeft(float deltaTime)
{
	mFacingDirection = FACING_LEFT;
	mPosition.x -= MovementSpeed * deltaTime;
}
void CharacterMario::MoveRight(float deltaTime)
{
	mFacingDirection = FACING_RIGHT;
	mPosition.x += MovementSpeed * deltaTime;
}

void CharacterMario::Gravity(float deltaTime)
{
	mPosition.y += GRAVITY * deltaTime;
	mCanJump = false;
}

Rect2D CharacterMario::GetCollisionBox()
{
	return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth(), mTexture->GetHeight());
}

bool CharacterMario::GetRespawning()
{
	return isRespawning;
}

bool CharacterMario::GetAlive()
{
	return isAlive;
}
int CharacterMario::GetLivesLeft()
{
	return livesLeft;
}

void CharacterMario::Kill()
{
	isAlive = false;
	Jump();
	mSounds->PlayToChannel(-1, "Music/MarioDie.wav", 0, true);
}
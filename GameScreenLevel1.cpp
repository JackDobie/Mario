#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include "PowBlock.h"
#include "Character.h"
#include "GameScreenManager.h"

vector<EnemyKoopa*> GameScreenLevel1::mEnemies;

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	mRenderer	= renderer;
	mLevelMap	= NULL;
	SetUpLevel();
}
GameScreenLevel1::~GameScreenLevel1()
{
	delete mBackgroundTexture;
	mBackgroundTexture = NULL;
	delete mCharacter1;
	mCharacter1 = NULL;
	delete mPowBlock;
	mPowBlock = NULL;
	mEnemies.clear();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	if (!Mix_PlayingMusic() && !endOfGame)
		mSounds->Play("Music/Mario.mp3", -1);

	//Do the screen shake if required
	if (mScreenshake)
	{
		mScreenshakeTime -= deltaTime;
		mWobble++;
		mBackgroundYPos = sin(mWobble);
		mBackgroundYPos *= 3.0f;
		//end the shake after the duration
		if (mScreenshakeTime <= 0.0f)
		{
			mScreenshake = false;
			mBackgroundYPos = 0.0f;
		}
	}

	//update the player
	mCharacter1->Update(deltaTime, e);

	UpdatePOWBlock();

	if (spawnedEnemies == L1_ENEMIES_MAX && mEnemies.size() == 0 && !gameOver && mCharacter1->GetAlive() && !mCharacter1->GetRespawning() && !endOfGame)
	{
		playerWon = true;
	}
	if (mEnemies.size() < L1_MAX_ONSCREEN_ENEMIES && spawnedEnemies < L1_ENEMIES_MAX)
	{
		if (spawnTimer >= 6.0f)
		{
			if (rand() % 2 == 0)
				CreateKoopa(Vector2D(20, 32), FACING_RIGHT, KOOPA_SPEED);
			else
				CreateKoopa(Vector2D(455, 32), FACING_LEFT, KOOPA_SPEED);
			spawnTimer = 0.0f;
		}
		else
			spawnTimer += deltaTime;
	}
	

	if (!mCharacter1->GetEndOfGame())
	{
		if(!endOfGame)
			UpdateEnemies(deltaTime, e);
	}
	else
		endOfGame = true;

	if (endOfGame)
	{
		gameOver = true;
	}

	if (gameOver)
	{
		mEnemies.clear();
		if(!Mix_Playing(-1))
			GameScreenManager::ChangeScreen(SCREEN_GAMEOVER);
	}

	if (playerWon && !gameOver && !endOfGame)
	{
		//mSounds->PlayToChannel(-1, "Music/Win.wav", 0, true);
		mEnemies.clear();
		GameScreenManager::ChangeScreen(SCREEN_LEVEL2);
		//go to next level
	}
}

void GameScreenLevel1::Render()
{
	for (unsigned int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i]->Render();
	}
	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE, 0.0f);
	mPowBlock->Render();
	mCharacter1->Render();
}

bool GameScreenLevel1::SetUpLevel()
{
	bool wonPrevious;
	if (Mix_PlayingMusic())
		wonPrevious = true;
	else
		wonPrevious = false;

	spawnTimer = 0.0f;
	endOfGame = false;
	gameOver = false;
	playerWon = false;
	spawnedEnemies = 0;

	//create the level map
	SetLevelMap();

	//set up sounds
	mSounds = new SoundEffect();

	Mix_HaltMusic();
	mSounds->Play("Music/Mario.mp3", -1);

	if (wonPrevious)
		mSounds->PlayToChannel(-1, "Music/Win.wav", 0, true);

	//set up the player
	mCharacter1 = new CharacterMario(mRenderer, "Images/Mario.png", Vector2D(64, 330), mLevelMap, mSounds);

	if (rand() % 2 == 0)
		CreateKoopa(Vector2D(20, 32), FACING_RIGHT, KOOPA_SPEED);
	else
		CreateKoopa(Vector2D(455, 32), FACING_LEFT, KOOPA_SPEED);
	//CreateKoopa(Vector2D(325, 32), FACING_LEFT, KOOPA_SPEED);

	//set up pow block
	mPowBlock = new PowBlock(mRenderer, mLevelMap);
	mScreenshake = false;
	mBackgroundYPos = 0.0f;

	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Images/BackgroundLevel1.png"))
	{
		cout << "Failed to load background texture!" << endl;
		return false;
	}
	else
		return true;
}

void GameScreenLevel1::SetLevelMap()
{
	int map[MAP_HEIGHT][MAP_WIDTH]	= { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0 },
										{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };
	//clear up old maps
	if (mLevelMap != NULL)
	{
		delete mLevelMap;
	}
	//set the new map
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel1::UpdatePOWBlock()
{
	//check collision
	if (Collisions::Instance()->Box(mCharacter1->GetCollisionBox(), mPowBlock->GetCollisionBox()))
	{
		if (mPowBlock->IsAvailable())
		{
			//collided whilst jumping
			if (mCharacter1->IsJumping())
			{
				DoScreenshake();
				mPowBlock->TakeAHit();
				mSounds->PlayToChannel(-1, "Music/POW.wav", 0, false);
				mCharacter1->CancelJump();
			}
		}
	}
}

void GameScreenLevel1::DoScreenshake()
{
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;
	for (unsigned int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i]->TakeDamage();
	}
}

void GameScreenLevel1::UpdateEnemies(float deltaTime, SDL_Event e)
{
	if (!mEnemies.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mEnemies.size(); i++)
		{
			if (mEnemies[i]->GetPosition().y > SCREEN_HEIGHT)
			{
				mEnemies[i]->Kill();
				enemyIndexToDelete = i;
			}
			//update
			mEnemies[i]->Update(deltaTime, e);
			//check to see if collides with player
			if (!mEnemies[i]->IsInPipe()) //ignore the collisions if the enemy is behind a pipe
			{
				if (mEnemies.size() > 1 && spawnedEnemies > 1)
				{
					//check enemy collision with enemies
					for (int j = 0; j < mEnemies.size(); j++)
					{
						if (j != i)
						{
							if (Collisions::Instance()->Box(mEnemies[i]->GetCollisionBox(), mEnemies[j]->GetCollisionBox()))
							{
								mEnemies[i]->Turn();
								mEnemies[j]->Turn();
							}
						}
					}
				}
				//check enemy collision with players
				if (!mCharacter1->GetRespawning())
				{
					if (Collisions::Instance()->Box(mEnemies[i]->GetCollisionBox(), mCharacter1->GetCollisionBox()))
					{
						if (!mEnemies[i]->GetInjured())
						{
							if(mCharacter1->GetAlive())
								mCharacter1->Kill();
							//mario is kil
						}
						else if (mEnemies[i]->GetInjured())
						{
							if (!mEnemies[i]->HasBeenHit())
							{
								mEnemies[i]->Kill();
								mSounds->PlayToChannel(-1, "Music/Kick.wav", 0, false);
							}
						}
					}
				}
			}

			//if the enemy is no longer alive, schedule it for deletion
			if (!mEnemies[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}
		//remove dead enemies: 1 each update
		if (enemyIndexToDelete != -1)
		{
			mEnemies.erase(mEnemies.begin() + enemyIndexToDelete);
		}
	}
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	EnemyKoopa* enemyKoopa = new EnemyKoopa(mRenderer, "Images/Koopa.png", mLevelMap, position, direction, speed, mSounds);
	spawnedEnemies++;
	mEnemies.push_back(enemyKoopa);
}
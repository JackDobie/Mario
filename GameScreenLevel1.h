#pragma once

#ifndef _GAMESCREENLEVEL1_H
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"
#include "CharacterMario.h"
#include "EnemyKoopa.h"
#include "Texture2D.h"
#include "Collisions.h"
#include "LevelMap.h"
#include "SoundEffect.h"

#include <iostream>
#include <vector>

class Texture2D;

class Character;
class EnemyKoopa;

class PowBlock;

class GameScreenLevel1 : GameScreen
{
private:
	Texture2D*	mBackgroundTexture;
	bool		SetUpLevel();
	bool		endOfGame;
	bool		gameOver;
	bool		playerWon;
	CharacterMario* mCharacter1;
	void		SetLevelMap();
	LevelMap*	mLevelMap;
	PowBlock*	mPowBlock;
	//screenshake
	bool	mScreenshake;
	float	mScreenshakeTime;
	float	mWobble;
	float	mBackgroundYPos;
	void	DoScreenshake();
	//enemy
	float	spawnTimer;
	int		spawnedEnemies;
	//sounds
	SoundEffect* mSounds;
public:
	GameScreenLevel1();
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1();

	void Render();
	void Update(float deltaTime, SDL_Event e);
	void UpdatePOWBlock();
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	static vector<EnemyKoopa*> mEnemies;
};
#endif // !_GAMESCREENLEVEL1_H
#pragma once

#ifndef _GAMESCREENLEVEL2_H
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"
#include "CharacterMario.h"
#include "Texture2D.h"
#include "Collisions.h"
#include "LevelMap.h"
#include "SoundEffect.h"
#include "EnemyCrab.h"
#include <iostream>
#include <vector>
class Texture2D;
class Character;
class PowBlock;

class GameScreenLevel2 : GameScreen
{
private:
	Texture2D*		mBackgroundTexture;
	bool			SetUpLevel();
	bool			endOfGame;
	bool			gameOver;
	bool			playerWon;
	CharacterMario* mCharacter1;
	void			SetLevelMap();
	LevelMap*		mLevelMap;
	PowBlock*		mPowBlock;
	bool			mScreenshake;
	float			mScreenshakeTime;
	float			mWobble;
	float			mBackgroundYPos;
	void			DoScreenshake();
	float			spawnTimer;
	int				spawnedEnemies;
	SoundEffect*	mSounds;
public:
	GameScreenLevel2(SDL_Renderer* renderer);
	~GameScreenLevel2();

	void	Render();
	void	Update(float deltaTime, SDL_Event e);
	void	UpdatePOWBlock();
	void	UpdateEnemies(float deltaTime, SDL_Event e);
	void	CreateCrab(Vector2D position, FACING direction, float speed);
	static	vector<EnemyCrab*> mEnemies;
};

#endif // !_GAMESCREENLEVEL2_H


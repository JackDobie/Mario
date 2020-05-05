#pragma once

#ifndef _GAMESCREENGAMEOVER_H
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"
#include "Texture2D.h"
#include "SoundEffect.h"
#include <iostream>
class Texture2D;

class GameScreenGameOver : GameScreen
{
private:
	SoundEffect* mSounds;
	Texture2D* mBackgroundTexture;
public:
	GameScreenGameOver();
	GameScreenGameOver(SDL_Renderer* renderer);
	~GameScreenGameOver();

	void Render();
	void Update(float deltaTime, SDL_Event e);
};

#endif // !_GAMESCREENGAMEOVER_H
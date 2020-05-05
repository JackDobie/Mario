#pragma once

#ifndef _GAMESCREENMANAGER_H
#include "SDL.h"
#include "Commons.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "GameScreenGameOver.h"
#include "GameScreen.h"

class GameScreen;

class GameScreenManager
{
private:
	SDL_Renderer* mRenderer;
	GameScreen* mCurrentScreen;
	SCREENS currentScreen;
public:
	GameScreenManager();
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	static void ChangeScreen(SCREENS newScreen);
	static SCREENS GetCurrentScreen();
};
#endif //!_GAMESCREENMANAGER_H
//#include "GameScreen.h"
#include "GameScreenManager.h"
#include "SDL.h"

GameScreenManager* gsm;

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	gsm = this;
	mCurrentScreen = NULL;
	mRenderer = renderer;
	ChangeScreen(startScreen);
}
GameScreenManager::~GameScreenManager()
{
	mRenderer = NULL;
	mCurrentScreen = NULL;
	delete mCurrentScreen;
}

void GameScreenManager::Render()
{
	mCurrentScreen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	mCurrentScreen->Update(deltaTime, e);
}

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	if (gsm->mCurrentScreen != NULL)
	{
		delete gsm->mCurrentScreen;
	}
	
	GameScreenLevel1*	level1;
	GameScreenLevel2*	level2;
	GameScreenGameOver*	gameover;

	switch (newScreen)
	{
	case SCREEN_LEVEL1:
		gsm->currentScreen = SCREEN_LEVEL1;
		level1 = new GameScreenLevel1(gsm->mRenderer);
		gsm->mCurrentScreen = (GameScreen*)level1;
		level1 = NULL;
		break;
	case SCREEN_LEVEL2:
		gsm->currentScreen = SCREEN_LEVEL2;
		level2 = new GameScreenLevel2(gsm->mRenderer);
		gsm->mCurrentScreen = (GameScreen*)level2;
		level2 = NULL;
		break;
	case SCREEN_GAMEOVER:
		gsm->currentScreen = SCREEN_GAMEOVER;
		GameScreenLevel1::mEnemies.clear();
		gameover = new GameScreenGameOver(gsm->mRenderer);
		gsm->mCurrentScreen = (GameScreen*)gameover;
		gameover = NULL;
		break;
	}
}

SCREENS GameScreenManager::GetCurrentScreen()
{
	return gsm->currentScreen;
}
#include "GameScreenGameOver.h"
#include "GameScreenManager.h"

GameScreenGameOver::GameScreenGameOver(SDL_Renderer* renderer) : GameScreen(renderer)
{
	mSounds = new SoundEffect();
	mRenderer = renderer;
	Mix_HaltMusic();
	mSounds->Play("Music/GameOver.wav", 1);
	mBackgroundTexture = new Texture2D(mRenderer);

	if (!mBackgroundTexture->LoadFromFile("Images/GameOver.png"))
	{
		cout << "Failed to load background texture!" << endl;
	}
}

GameScreenGameOver::~GameScreenGameOver()
{
	mRenderer = NULL;
	delete mRenderer;
	mBackgroundTexture = NULL;
	delete mBackgroundTexture;
}

void GameScreenGameOver::Render()
{
	mBackgroundTexture->Render(Vector2D(0, 0), SDL_FLIP_NONE, 0);
}

void GameScreenGameOver::Update(float deltaTime, SDL_Event e)
{
	if (!Mix_PlayingMusic() && !Mix_Playing(-1))
	{
		if(e.type == SDL_KEYDOWN)
			GameScreenManager::ChangeScreen(SCREEN_LEVEL1);//restart when press button
	}
}
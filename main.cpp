#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Constants.h"
#include "Commons.h"
#include "Texture2D.h"
#include "GameScreenManager.h"
#include "SoundEffect.h"
#include <iostream>
using namespace::std;

SDL_Window*			gWindow	= NULL;
SDL_Renderer*		gRenderer = NULL;
GameScreenManager*	gameScreenManager;
Uint32				gOldTime;

bool InitSDL();
void CloseSDL();
bool Update();
void Render();

int main(int argc, char* args[])
{
	if (InitSDL())
	{
		cout << "initSDL true" << endl;
	}

	gameScreenManager = new GameScreenManager(gRenderer, SCREEN_LEVEL1);
	gOldTime = SDL_GetTicks();
	while (!Update())//when update is true, quit
	{
		Render();
	}
	cout << "close" << endl;

	CloseSDL();//close window and free resources

	return 0;
}

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Mario",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									SCREEN_WIDTH,
									SCREEN_HEIGHT,
									SDL_WINDOW_SHOWN);
		if (gWindow == NULL) //window was not created
		{
			cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}

		//if gets this far, would have been successful
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer != NULL)
		{
			//initialise png load
			int imageFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imageFlags) & imageFlags))
			{
				cout << "SDL_Image could not be initialised. Error: " << IMG_GetError();
				return false;
			}
		}
		else
		{
			cout << "Renderer could not initialise. Error: " << IMG_GetError();
			return false;
		}

		//Initialise the Mixer
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			cout << "Mixer could not initialise. Error: " << Mix_GetError();
			return false;
		}

		return true;
	}
}

void Render()
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(gRenderer);

	gameScreenManager->Render();

	SDL_RenderPresent(gRenderer);
}

void CloseSDL()
{
	//destroy the game screen manager
	delete gameScreenManager;
	gameScreenManager = NULL;

	//release the renderer
	gRenderer = NULL;
	SDL_DestroyRenderer(gRenderer);

	//release the window
	gWindow = NULL;
	SDL_DestroyWindow(gWindow);

	//quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool Update()
{
	//get the new time
	Uint32 newTime = SDL_GetTicks();

	//event handler
	SDL_Event e;

	//get the events
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			CloseSDL();
			break;
		}
	}

	if (Mix_Playing(-1) == 0)
		Mix_ResumeMusic();

	gameScreenManager->Update((float)(newTime - gOldTime) / 1000.0f, e);

	//set the current time to be the old time to get diff between frames
	gOldTime = newTime;

	return false;
}
#include "SoundEffect.h"

SoundEffect::SoundEffect()
{
	gMusic = NULL;
	sound = NULL;
}

SoundEffect::~SoundEffect()
{
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
	delete gMusic;
	Mix_FreeChunk(sound);
	sound = NULL;
	delete sound;
}

void SoundEffect::Play(string path, int loops)
{
	if (gMusic != NULL)
		gMusic = NULL;
	gMusic = Mix_LoadMUS(path.c_str());
	if (gMusic == NULL)
	{
		cout << "Failed to load sound! Error: " << Mix_GetError() << endl;
	}
	else
	{
		if (Mix_PlayingMusic() == 0)
		{
			if (Mix_PlayMusic(gMusic, loops) < 0)
			{
				cout << "Error playing sound!" << endl;
			}
		}
	}
}

void SoundEffect::PlayToChannel(int channel, string path, int loops, bool pauseMusic)
{
	if (sound != NULL)
	{
		sound = NULL;
		delete sound;
	}
	if(pauseMusic)
		Mix_PauseMusic();

	sound = Mix_LoadWAV(path.c_str());
	if (sound == NULL)
	{
		cout << "Failed to load sound! Error: " << Mix_GetError() << endl;
	}
	if (Mix_PlayChannel(channel, sound, loops) < 0)
	{
		cout << "Error playing sound! Error: " << Mix_GetError() << endl;
	}
}
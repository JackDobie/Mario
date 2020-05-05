#pragma once

#ifndef _SOUNDEFFECT_H

#include <SDL_mixer.h>
#include <iostream>
using namespace::std;

class SoundEffect
{
private:
	Mix_Music* gMusic	= NULL;
	Mix_Chunk* sound	= NULL;
public:
	SoundEffect();
	~SoundEffect();
	void Play(string path, int loops);
	void PlayToChannel(int channel, string path, int loops, bool pauseMusic);
};

#endif // !_SOUNDEFFECT_H

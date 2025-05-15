#include "sound.h"

namespace Tmpl8
{
	Audio::Audio()
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);


		song1 = Mix_LoadMUS("assets/Music/Music1.mp3");
		if (song1 == nullptr)
			SDL_Log("Failed to load music: %s\n", Mix_GetError());


		Click_Sound = Mix_LoadWAV("assets/Sound/ClickSound.wav");
		if (Click_Sound == nullptr)
			SDL_Log("Failed to load click sound: %s\n", Mix_GetError());

		Hover_Sound = Mix_LoadWAV("assets/Sound/HoverSound.wav");
		if (Hover_Sound == nullptr)
			SDL_Log("Failed to load hover sound: %s\n", Mix_GetError());

		Coin_Sound = Mix_LoadWAV("assets/Sound/CoinSFX.wav");
		if (Coin_Sound == nullptr)
			SDL_Log("Failed to load coin sound: %s\n", Mix_GetError());

		Mix_Volume(-1, 30);
		Mix_VolumeMusic(30);
		Mix_PlayMusic(song1, -1);

	}

	Audio::~Audio()
	{
		Mix_FreeMusic(song1);
		Mix_FreeChunk(Click_Sound);
		Mix_FreeChunk(Hover_Sound);
		Mix_CloseAudio();
	}

	void Audio::PlayClickSound()
	{
		Mix_PlayChannel(-1, Click_Sound, 0);
	}

	void Audio::PlayHoverSound()
	{
		Mix_PlayChannel(-1, Hover_Sound, 0);
	}
	void Audio::PlayCoinSound()
	{
		Mix_PlayChannel(-1, Coin_Sound, 0);
	}
}



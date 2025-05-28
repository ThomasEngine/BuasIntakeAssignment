#include "sound.h"

namespace Tmpl8
{
	Audio::Audio()
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024); // (Initializing audio)

		// Loading Music and sfx
		m_BackgroundMusic = Mix_LoadMUS("assets/Music/Music1.mp3");
		if (m_BackgroundMusic == nullptr)
			SDL_Log("Failed to load music: %s\n", Mix_GetError());


		m_ClickSound = Mix_LoadWAV("assets/Sound/ClickSound.wav");
		if (m_ClickSound == nullptr)
			SDL_Log("Failed to load click sound: %s\n", Mix_GetError());

		m_HoverSound = Mix_LoadWAV("assets/Sound/HoverSound.wav");
		if (m_HoverSound == nullptr)
			SDL_Log("Failed to load hover sound: %s\n", Mix_GetError());

		m_CoinSound = Mix_LoadWAV("assets/Sound/CoinSFX.wav");
		if (m_CoinSound == nullptr)
			SDL_Log("Failed to load coin sound: %s\n", Mix_GetError());

		// Setings volume
		Mix_Volume(-1, 30);
		Mix_VolumeMusic(25);
		
		// Player background music
		Mix_PlayMusic(m_BackgroundMusic, -1);

	}

	Audio::~Audio()
	{
		Mix_FreeMusic(m_BackgroundMusic);
		Mix_FreeChunk(m_ClickSound);
		Mix_FreeChunk(m_HoverSound);
		Mix_CloseAudio();
	}

	void Audio::PlayClickSound()
	{
		Mix_PlayChannel(-1, m_ClickSound, 0);
	}

	void Audio::PlayHoverSound()
	{
		Mix_PlayChannel(-1, m_HoverSound, 0);
	}
	void Audio::PlayCoinSound()
	{
		Mix_PlayChannel(-1, m_CoinSound, 0);
	}
}



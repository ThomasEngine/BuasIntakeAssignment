#pragma once

#include <SDL.h>
#include <SDL_mixer.h>


namespace Tmpl8
{
	class Audio
	{
	public:
		Audio();
		~Audio();

		void PlayClickSound();
		void PlayHoverSound();
		void PlayCoinSound();

	private:
		Mix_Music* m_BackgroundMusic;
		Mix_Chunk* m_ClickSound;
		Mix_Chunk* m_HoverSound;
		Mix_Chunk* m_CoinSound;
	};
}
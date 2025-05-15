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
		Mix_Music* song1;
		Mix_Chunk* Click_Sound;
		Mix_Chunk* Hover_Sound;
		Mix_Chunk* Coin_Sound;
	};
}
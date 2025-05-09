#pragma once
#include <SDL.h>
#include <iostream>

namespace Tmpl8
{

	class Object
	{
	public:

		Object();
		SDL_Rect GetDest() const { return dest; }
		SDL_Rect GetSource() const { return src; }
		SDL_Texture* GetTex() const { return tex; }

		void SetDest(int x, int y, int w, int h);
		void SetSource(int x, int y, int w, int h);
		void SetImage(std::string filename, SDL_Renderer* ren, int color);
		void SetSolid(bool s) { is_Solid = s; }
		bool GetSolid() { return is_Solid; }

	private:
		SDL_Rect dest;
		SDL_Rect src;
		SDL_Texture* tex;
		bool is_Solid;
	};
}
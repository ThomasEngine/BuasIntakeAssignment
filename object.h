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
		void SetDest(int x, int y);
		void SetSource(int x, int y, int w, int h);
		void SetImage(std::string filename, SDL_Renderer* ren, int color);
		void SetSolid(bool s) { is_Solid = s; }
		bool GetSolid() const { return is_Solid; }
		int GetDX() const { return dest.x; }
		int GetDY() const { return dest.y; }
		int GetDW() const { return dest.w; }
		int GetDH() const { return dest.h; }
		void SetDY(int y) { dest.y = y; }

		void DrawRect(SDL_Rect rect, SDL_Renderer* ren) const;

	private:
		SDL_Rect dest;
		SDL_Rect src;
		SDL_Texture* tex;
		bool is_Solid;
	};
}
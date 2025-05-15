#pragma once
#include <SDL.h>
#include <iostream>

namespace Tmpl8
{

	class Object
	{
	public:

		Object();
		SDL_Rect GetDest() const { return m_Dest; }
		SDL_Rect GetSource() const { return m_Src; }
		SDL_Texture* GetTex() const { return m_Tex; }

		void SetDest(int x, int y, int w, int h);
		void SetDest(int x, int y);
		void SetSource(int x, int y, int w, int h);
		void SetImage(std::string filename, SDL_Renderer* ren, int color);
		void SetTexture(SDL_Texture* texture) { m_Tex = texture; }
		void SetSolid(bool s) { m_IsSolid = s; }
		bool GetSolid() const { return m_IsSolid; }
		int GetDX() const { return m_Dest.x; }
		int GetDY() const { return m_Dest.y; }
		int GetDW() const { return m_Dest.w; }
		int GetDH() const { return m_Dest.h; }
		void SetDY(int y) { m_Dest.y = y; }

		void DrawRect(SDL_Rect rect, SDL_Renderer* ren) const;

	private:
		SDL_Rect m_Dest;
		SDL_Rect m_Src;
		SDL_Texture* m_Tex;
		bool m_IsSolid;
	};
}
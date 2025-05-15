#include "object.h"
#include <SDL_image.h>

namespace Tmpl8 {
	Object::Object() : m_Tex(nullptr),  m_IsSolid(true) {}

	void Object::SetDest(int x, int y, int w, int h)
	{
		m_Dest.x = x;
		m_Dest.y = y;
		m_Dest.w = w;
		m_Dest.h = h;
	}

	void Object::SetDest(int x, int y)
	{
		m_Dest.x = x;
		m_Dest.y = y;
	}

	void Object::SetSource(int x, int y, int w, int h)
	{
		m_Src.x = x;
		m_Src.y = y;
		m_Src.w = w;
		m_Src.h = h;
	}

	void Object::SetImage(std::string filename, SDL_Renderer* ren, int color) // color is 0 for black and 1 for white
	{
		SDL_Surface* surf = IMG_Load(filename.c_str());

		if (!surf)
		{
			std::cerr << "Failed to load image: " << filename << std::endl;
			return;
		}

		m_Tex = SDL_CreateTextureFromSurface(ren, surf);

		SDL_FreeSurface(surf);
	}
	void Object::DrawRect(SDL_Rect rect, SDL_Renderer* ren) const
	{
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // Red color
		SDL_RenderDrawRect(ren, &rect);
	}
}

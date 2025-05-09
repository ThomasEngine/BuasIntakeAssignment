#include "object.h"

namespace Tmpl8 {
	Object::Object() : tex(nullptr),  is_Solid(1) {}

	void Object::SetDest(int x, int y, int w, int h)
	{
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
	}

	void Object::SetSource(int x, int y, int w, int h)
	{
		src.x = x;
		src.y = y;
		src.w = w;
		src.h = h;
	}

	void Object::SetImage(std::string filename, SDL_Renderer* ren, int color) // color is 0 for black and 1 for white
	{
		SDL_Surface* surf = SDL_LoadBMP(filename.c_str());

		if (!surf)
		{
			std::cerr << "Failed to load image: " << filename << std::endl;
			return;
		}
		// Set the color key (e.g., magenta: RGB(255, 0, 255))
		SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255*color, 255*color, 255*color));


		tex = SDL_CreateTextureFromSurface(ren, surf);

		SDL_FreeSurface(surf);
	}
}

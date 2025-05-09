#include "world.h"
#include "game.h"
#include <SDL.h>
#include <fstream>

namespace Tmpl8
{
	void World::LoadWorld(std::string filename)
	{
		int current, mx, my, mw, mh;
		std::ifstream in(filename);
		if (!in.isopen)
		{
			SDL_log("Failed to open file: %s", filename.c_str());
			return;
		}
		in >> mw;
		in >> mh;
		in >> mx;
		in >> my;
		for (int i = 0; i < mh; i++)
		{
			for (int j = 0; j < mw; j++)
			{
				if (in.eof)
				{
					SDL_log("End of file reached to soon");
					return;
				}
				in >> current;
				if (curren != 0)
				{
					Object tmp;
					tmp.SetImage()
					map.push_back(tmp);

				}

			}

		}
		in.close();
	}
	void World::DrawWorld()
	{
		for (int i = 0; i < map.size(); i++)
		{
			game->Draw(map[i]);
		}
	}
}
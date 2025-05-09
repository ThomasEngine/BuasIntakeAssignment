#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <iostream> //std::cout
#include <sstream>
#include <fstream>

namespace Tmpl8
{
	void Game::Init()
	{
		player.SetImage("assets/Run (32x32).bmp", m_renderer, 0);
		player.SetDest(200, 200, 32*2, 32*2);
		idol = player.createCycle(0, 32, 32, 12, 4);

		Background.SetImage("assets/background/Background_2.bmp", m_renderer, 0);
		Background.SetSource(0, 0, 800, 600);
		Background.SetDest(0, 0, 800, 600);

		loadMap("level.txt");


		
	}
	void Game::Shutdown()
	{
		SDL_DestroyTexture(Background.GetTex());
		SDL_DestroyTexture(player.GetTex());
		for (int i = 0; i < m_map.size(); i++)
		{
			SDL_DestroyTexture(m_map[i].GetTex());
		}
		m_map.clear();
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		std::cout << "Game Shutdown" << std::endl;
		std::cout << "Bye!";
	}




	void Game::Tick(float deltaTime)
	{
		// clear the graphics window
		m_screen->Clear(0);
		SDL_RenderClear(m_renderer);

		Update(deltaTime);

		Render(deltaTime);
	}

	void Game::Update(float deltaTime)
	{
		player.updateAnimation();
	}

	void Game::Render(float deltaTime)
	{
		DrawAll();
		SDL_RenderPresent(m_renderer);
	}

	void Game::Draw(Object o)
	{
		SDL_Rect dest = o.GetDest();
		SDL_Rect src = o.GetSource();
		SDL_RenderCopyEx(m_renderer, o.GetTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}

	void Game::DrawAll()
	{
		// Draw all objects here
		Draw(Background);
		drawMap();
		Draw(player);
	}
	
	void Game::loadMap(std::string filename)
	{
		m_map.clear();

		int current, mx, my, mw, mh;
		std::ifstream in(filename);
		if (!in.is_open())
		{
			SDL_Log("Failed to open file: %s", filename.c_str());
			return;
		}
		in >> mw;
		in >> mh;
		in >> mx;
		in >> my;
		for (int i = 0; i < mw; i++)
		{
			for (int j = 0; j < mh; j++)
			{
				if (in.eof())
				{
					SDL_Log("End of file reached to soon");
					return;
				}
				in >> current;
				if (current != 0)
				{
					Object tmp;
					tmp.SetImage("assets/tilemap/Assets2.bmp", m_renderer, 1);
					tmp.SetSource((current - 1) * TILESIZE, 0, TILESIZE, TILESIZE);
					tmp.SetDest((i * TILESIZE) * mx, (j * TILESIZE) * my, TILESIZE, TILESIZE);
					if (current == 0 || current == 3) tmp.SetSolid(true);
					m_map.push_back(tmp);
				}
			}
		}
		in.close();
	}

	void Game::drawMap()
	{
		for (int i = 0; i < m_map.size(); i++)
		{
			Draw(m_map[i]);
		}
	}


	void Game::showFPS(float deltaTime)
	{
		// Calculate FPS
		float fps = 1000 / deltaTime;

		// Convert FPS to string
		std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));

		// Display FPS on the screen

	}
};
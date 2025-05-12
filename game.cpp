#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <iostream> //std::cout
#include <sstream>
#include <fstream>
#include "levels.h"
#include "template.h"

namespace Tmpl8
{
	void Game::Init()
	{
		player = new Player(200, 200, m_renderer);

		Background.SetImage("assets/background/Background_2.png", m_renderer, 0);
		Background.SetSource(0, 0, 1280, 720);
		Background.SetDest(0, 0, 1280, 720);

		m_TileMap = new World(m_renderer, TILEMAP_ROWS, TILEMAP_COLS, TILESIZE);
		m_TileMap->BuildTileMap();
		Mapx = Mapy = 0;
	}
	void Game::Shutdown()
	{
		delete player;
		delete m_TileMap;
		SDL_DestroyTexture(Background.GetTex());
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
	}
	void Game::Tick(float deltaTime)
	{
		// clear the graphics window
		SDL_RenderClear(m_renderer);
		Update(deltaTime);
		Render(deltaTime);
	}
	void Game::Update(float deltaTime)
	{
		UpdateCollision(player, m_TileMap);
		player->Update(deltaTime);
	}
	void Game::Render(float deltaTime)
	{
		DrawAll();
		SDL_RenderPresent(m_renderer);
		showFPS(deltaTime);
	}

	void Game::Draw(Object o)
	{
		if (!o.GetTex())
		{
			SDL_Log("Texture not loaded\n");
			return;
		}
		SDL_Rect dest = o.GetDest();
		SDL_Rect src = o.GetSource();
		SDL_RenderCopyEx(m_renderer, o.GetTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}

	void Game::UpdateCollision(Player* player, const World* world)
	{
		const auto& map = world->GetMap(); // Get the tile map from the world

		for (auto& tile : map)
		{
			if (CheckCollision(*player, tile))
			{
				if (tile.GetSolid())
				{
				//player->SetPos(player->GetDX(), tile.GetDY() - TILESIZE - 20);
					player->SetYPos(tile.GetDY() - TILESIZE - 20);
					player->SetFall(false);
				}
				return;
			}
			else
			{
				player->SetFall(true);
			}
		}
	}

	bool Game::CheckCollision(const Player& a, const Object& b)
	{
		//return (a.GetDX() < (b.GetDX() + b.GetDW())) &&
		//	((a.GetDX() + a.GetDW()) > b.GetDX()) &&
		//	(a.GetDY() < (b.GetDY() + b.GetDH())) &&
		//	((a.GetDY() + a.GetDH()) > b.GetDY());
		return SDL_HasIntersection(&a.getRect(), &b.GetDest());
	}

	void Game::DrawAll()
	{
		// Draw all objects here
		Draw(Background);
		m_TileMap->DrawTileMap(m_renderer, Mapx, Mapy, ScreenWidth, ScreenHeight);
		Draw(*player);
		player->DrawRect(player->getRect(), m_renderer);
	}

	void Game::showFPS(float deltaTime)
	{
		// Calculate FPS
		float fps = 1000 / deltaTime;

		// Convert FPS to string
		std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));

		// Display FPS on the screen
		std::cout << fps << std::endl;

	}
};
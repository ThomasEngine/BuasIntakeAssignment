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
		m_TileMap = new World(m_renderer, TILESIZE);
		m_TileMap->LoadTilemapFromFile("assets/tilemap/level1.txt");
		m_TileMap->BuildTileMap();

		player = new Player(16*32, 200, m_renderer);

		Background.SetImage("assets/background/Background_2.png", m_renderer, 0);
		Background.SetSource(0, 0, 1280, 720);
		Background.SetDest(0, 0, 1280, 720);


		cameraX = 0.f;
		//cameraY = player->GetDY() - ScreenHeight / 2;
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
		player->Update(deltaTime, m_TileMap);
		UpdateCameraY();
	}
	void Game::UpdateCameraY()
	{
		// Center camera on player
		float targetCameraY = player->GetDY() - ScreenHeight / 2;

		// Clamp to map bounds
		float maxCameraY = m_TileMap->GetRows() * TILESIZE - ScreenHeight;
		if (targetCameraY < 0) targetCameraY = 0;
		if (targetCameraY > maxCameraY) targetCameraY = maxCameraY;

		// Smooth scrolling (lerp)
		float cameraSpeed = 0.1f; // Adjust for smoothness
		cameraY += (targetCameraY - cameraY) * cameraSpeed;
	}
	void Game::Render(float deltaTime)
	{
		DrawAll();
		SDL_RenderPresent(m_renderer);
		showFPS(deltaTime);
	}

	void Game::DrawBackground(Object o)
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

	void Game::Draw(Object o)
	{
		if (!o.GetTex())
		{
			SDL_Log("Texture not loaded\n");
			return;
		}
		SDL_Rect dest = o.GetDest();
		dest.y -= static_cast<int>(cameraY); // Offset by camera
		SDL_Rect src = o.GetSource();
		SDL_RenderCopyEx(m_renderer, o.GetTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}

	void Game::DrawAll()
	{
		// Draw all objects here
		DrawBackground(Background);
		m_TileMap->DrawTileMap(m_renderer, cameraX, cameraY, ScreenWidth, ScreenHeight, player->GetChangeY());
		Draw(*player);

		//player->DrawRect(player->getRect(), m_renderer);
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
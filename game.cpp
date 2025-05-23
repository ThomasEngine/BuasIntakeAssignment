#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <iostream> //std::cout
#include <sstream>
#include <fstream>
#include "template.h"

namespace Tmpl8
{
	void Game::Init()
	{
		m_TileMap = new World(m_Renderer, TILESIZE);
		m_TileMap->LoadTilemapFromFile("assets/tilemap/level1.txt");
		m_TileMap->BuildTileMap();

		m_Player = new Player(64, 95 * 32, m_Renderer, m_Audio);
		m_Player->resetPlayer();

		m_Background.SetImage("assets/background/Background_2.png", m_Renderer, 0);
		m_Background.SetSource(0, 0, 1280, 720);
		m_Background.SetDest(0, 0, 1280, 720);

		m_Menu = new GameMenu(m_Renderer);
		m_State = GameState::Paused;

		m_CameraX = 0.f;
	}
	void Game::Shutdown()
	{
		delete m_Player;
		delete m_TileMap;
		delete m_Menu;
		SDL_DestroyTexture(m_Background.GetTex());
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyWindow(m_Window);
	}

	void Game::Restart()
	{
		m_Player->resetPlayer();
		m_PlayerTimer = 0.f;

		delete m_TileMap;

		m_PlayerFinished = false;

		m_TileMap = new World(m_Renderer, TILESIZE);
		m_TileMap->LoadTilemapFromFile("assets/tilemap/level1.txt");
		m_TileMap->BuildTileMap();
	}

	void Game::Tick(float deltaTime)
	{
		// clear the graphics window
		SDL_RenderClear(m_Renderer);

		switch (m_State)
		{
		case GameState::Paused: // Game is paused
			m_Menu->Render();
			break;
		case GameState::Playing: // Game is playing
			Update(deltaTime);
			Render(deltaTime);
			break;
		}
	}
	void Game::Update(float deltaTime)
	{
		m_Player->Update(deltaTime, m_TileMap);
		m_TileMap->UpdateCoinAnimation();
		UpdateCameraY();
		UpdateTimer(deltaTime);
	}
	void Game::UpdateCameraY()
	{
		// Center camera on player
		float targetCameraY = m_Player->GetDY() - ScreenHeight / 2;

		// Dont let camera go further then the map
		float maxCameraY = m_TileMap->GetRows() * TILESIZE - ScreenHeight;
		if (targetCameraY < 0) targetCameraY = 0;
		if (targetCameraY > maxCameraY) targetCameraY = maxCameraY;

		// Smooth scrolling
		float cameraSpeed = 0.1f; // Smoothness
		m_CameraY += (targetCameraY - m_CameraY) * cameraSpeed;
	}
	void Game::Render(float deltaTime)
	{
		DrawAll();
		SDL_RenderPresent(m_Renderer);
	}

	void Game::DrawStatic(Object o) // Draw function for static non moving objects.
	{
		if (!o.GetTex())
		{
			SDL_Log("Texture not loaded\n");
			return;
		}
		SDL_Rect dest = o.GetDest();
		SDL_Rect src = o.GetSource();
		SDL_RenderCopyEx(m_Renderer, o.GetTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}

	void Game::UpdateTimer(float deltaTime)
	{
		if (m_State == GameState::Playing && !m_TimerActive)
		{
			m_PlayerTimer = 0.0f;
			m_TimerActive = true;
		}
		if (m_State == GameState::Playing && m_TimerActive && !m_PlayerFinished)
			m_PlayerTimer += deltaTime / 1000.f;
		int px = m_Player->GetDX();
		int py = m_Player->GetDY();
		if (px >1120 && py <= 1578 && m_Player->GetAmountCoins() == 6 ) // check if player is finished
		{
			m_PlayerFinished = true;
			m_TimerActive = false;			
			m_Menu->SetGameState(GameState::Paused);
			m_State = GameState::Paused;
			m_Menu->SetMenu(MenuType::Victory);
		}
	}

	void Game::Draw(Object* o)
	{
		if (!o->GetTex())
		{
			SDL_Log("Texture not loaded\n");
			return;
		}
		SDL_Rect dest = o->GetDest();
		dest.y -= m_CameraY; // Offset by camera
		SDL_Rect src = o->GetSource();
		SDL_RenderCopyEx(m_Renderer, o->GetTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}

	void Game::DrawAll()
	{
		// Draw all objects here
		DrawStatic(m_Background);
		m_TileMap->DrawTileMap(m_Renderer, m_CameraX, m_CameraY, ScreenWidth, ScreenHeight);
		Draw(m_Player);
	}
}
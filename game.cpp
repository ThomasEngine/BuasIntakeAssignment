#include "game.h"

namespace Tmpl8
{
	void Game::Init()
	{
		m_TileMap = std::make_unique<World>(m_Renderer, TILESIZE);
		m_TileMap->LoadTilemapFromFile("assets/tilemap/level1.txt");
		m_TileMap->BuildTileMap();

		m_PlayerStartPos = m_TileMap->GetPlayerStartingPos();
		m_Player = std::make_unique<Player>(m_PlayerStartPos.x, m_PlayerStartPos.y + TILESIZE, m_Renderer, m_Audio); 
		m_Player->resetPlayer(m_PlayerStartPos);

		m_Background.SetImage("assets/background/Background_2.png", m_Renderer, 0);
		m_Background.SetSource(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		m_Background.SetDest(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		m_Menu = std::make_unique<GameMenu>(m_Renderer);
		m_State = GameState::Paused;

		m_Font = TTF_OpenFont("assets/fonts/basicFont.ttf", 24);


		m_CameraX = 0.f;
	}
	void Game::Shutdown()
	{
		m_TileMap.reset();
		m_Player.reset();
		m_Menu.reset();
		if (m_Background.GetTex()) SDL_DestroyTexture(m_Background.GetTex());
		if (m_Renderer) SDL_DestroyRenderer(m_Renderer);
		if (m_Window) SDL_DestroyWindow(m_Window);
		if (m_Font) TTF_CloseFont(m_Font);
	}

	void Game::Restart()
	{
		// Remove old tilemap
		m_TileMap.reset();

		// Build new Tilemap
		m_TileMap = std::make_unique<World>(m_Renderer, TILESIZE);
		m_TileMap->LoadTilemapFromFile("assets/tilemap/level1.txt");
		m_TileMap->BuildTileMap();

		m_PlayerStartPos = m_TileMap->GetPlayerStartingPos();
		m_Player->resetPlayer(m_PlayerStartPos);
		m_PlayerTimer = 0.f;
		m_PlayerFinished = false;
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
		m_Player->Update(deltaTime, m_TileMap.get());
		m_TileMap->UpdateCoinAnimation();
		CheckPlayerOutOfScreen();
		UpdateCameraY();
		UpdateTimer(deltaTime);
	}
	void Game::Render(float deltaTime)
	{
		DrawAll();
		SDL_RenderPresent(m_Renderer);
	}
	
	void Game::DrawStatic(const Object& o) // Draw function for static non moving objects.
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

	void Game::DrawStats()
	{
		// Create color white
		SDL_Color white = { 255,255,255,255 };

		// Showing the timer
		// Convert Time to string
		std::string TimeText = "TIME: " + std::to_string(static_cast<int>(m_PlayerTimer)); // Don't have wifi in the plane and I dont know out of my head how to make a string a const char*. So this is it for now
		// Render to screen
		RenderText("TimeText", 10, 10, white);

		// Showing amount of coins
		RenderText("Coins: X X X X X X ", SCREEN_WIDTH - 220, 10, white);
	}

	void Game::DrawAll()
	{
		// Draw all objects here
		DrawStatic(m_Background);
		m_TileMap->DrawTileMap(m_Renderer, m_CameraX, m_CameraY, SCREEN_WIDTH, SCREEN_HEIGHT);
		Draw(m_Player.get());
		DrawStats();
	}

	void Game::RenderText(const char* text, int x, int y, SDL_Color color) {
		SDL_Surface* surf = TTF_RenderText_Blended(m_Font, text, color);
		SDL_Texture* tex = SDL_CreateTextureFromSurface(m_Renderer, surf);
		SDL_Rect dst = { x, y, surf->w, surf->h };
		SDL_FreeSurface(surf);
		SDL_RenderCopy(m_Renderer, tex, nullptr, &dst);
		SDL_DestroyTexture(tex);
	}

	void Game::UpdateTimer(float deltaTime)
	{
		if (m_State == GameState::Playing && !m_TimerActive)
		{
			m_PlayerTimer = 0.0f;
			m_TimerActive = true;
		}
		if (m_State == GameState::Playing && m_TimerActive && !m_PlayerFinished)
			m_PlayerTimer += deltaTime / 1000.f; // in seconds

		if (IsPlayerFinished())
		{
			m_PlayerFinished = true;
			m_TimerActive = false;
			m_Menu->SetGameState(GameState::Paused);
			m_State = GameState::Paused;
			m_Menu->SetMenu(MenuType::Victory);
		}
	}
	void Game::UpdateCameraY()
	{
		// Center camera on player
		float targetCameraY = m_Player->GetDY() - SCREEN_HEIGHT / 2;

		// Dont let camera go further then the map
		float maxCameraY = m_TileMap->GetRows() * TILESIZE - SCREEN_HEIGHT;
		if (targetCameraY < 0) targetCameraY = 0;
		if (targetCameraY > maxCameraY) targetCameraY = maxCameraY;

		// Smooth scrolling
		float cameraSpeed = 0.1f; // Smoothness
		m_CameraY += (targetCameraY - m_CameraY) * cameraSpeed;
	}

	bool Game::IsPlayerFinished() // The player is at the finish line and collected all the coins.
	{
		SDL_Rect playerRect = m_Player->GetDest();
		SDL_Rect flagRect = m_TileMap->GetFlagRect();
		return (SDL_HasIntersection(&playerRect, &flagRect) && m_TileMap->GetCoins().empty());
	}

	void Game::CheckPlayerOutOfScreen()
	{
		if (m_Player->GetDY() > m_TileMap->GetRows() * TILESIZE )
		{
			Restart();
		}
	}

}
#pragma once
#include "object.h"
#include "entity.h"
#include <vector>
#include "world.h"
#include "player.h"
#include "gameMenu.h"



namespace Tmpl8 {


	class Surface;
	class Game
	{
	public:
		void SetTarget(Surface* surface, SDL_Renderer* renderer, SDL_Window* window) { m_screen = surface, m_renderer = renderer, m_window = window; }
		void Init();
		void Shutdown();

		void Tick(float deltaTime);
		void Update(float deltaTime);
		void UpdateCameraY();
		void Render(float deltaTime);
		void showFPS(float deltaTime);
		void DrawAll();
		void Draw(Object o);
		void DrawStatic(Object o);
		void ResetPlayer() { player->resetPlayer(); }

		GameMenu* GetMenu() { return m_menu; }
		GameState GetState() const { return static_cast<GameState>(m_state); }
		void SetState(GameState state) { m_state = state; }


		// Send keys to the player class
		void KeyUp(int key) { player->KeyUp(key); }
		void KeyDown(int key) { player->KeyDown(key); }
	private:
		Surface* m_screen;
		SDL_Renderer* m_renderer;
		SDL_Window* m_window;
		Object Background;
		World* m_TileMap;
		Player* player;
		GameMenu* m_menu;
		GameState m_state = GameState::Paused;
		float cameraX, cameraY;
	};

} // namespace Tmpl8
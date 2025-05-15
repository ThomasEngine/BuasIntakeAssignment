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
		void SetTarget(Surface* surface, SDL_Renderer* renderer, SDL_Window* window, Audio* audio) { m_Screen = surface, m_Renderer = renderer, m_Window = window, m_Audio = audio; }
		void Init();
		void Shutdown();
		void Restart();

		void Tick(float deltaTime);
		void Update(float deltaTime);
		void UpdateCameraY();
		void Render(float deltaTime);
		void showFPS(float deltaTime);
		void DrawAll();
		void Draw(Object* o);
		void DrawStatic(Object o);
		void ResetPlayer() { m_Player->resetPlayer(); }

		GameMenu* GetMenu() { return m_Menu; }
		GameState GetState() const { return static_cast<GameState>(m_State); }
		void SetState(GameState state) { m_State = state; }
		void UpdateTimer(float deltaTime);


		// Send keys to the player class
		void KeyUp(int key) { m_Player->KeyUp(key); }
		void KeyDown(int key) { m_Player->KeyDown(key); }
	private:
		Surface* m_Screen;
		SDL_Renderer* m_Renderer;
		SDL_Window* m_Window;
		Audio* m_Audio;
		Object m_Background;
		World* m_TileMap;
		Player* m_Player;
		GameMenu* m_Menu;
		GameState m_State = GameState::Paused;
		float m_CameraX, m_CameraY;
		float m_PlayerTimer = 0.0f;
		bool m_TimerActive = false;
		bool m_PlayerFinished = false;
	};

} // namespace Tmpl8
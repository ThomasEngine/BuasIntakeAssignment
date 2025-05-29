#pragma once
#include "entity.h"
#include "gameMenu.h"
#include <memory>
#include "object.h"
#include "player.h"
#include "template.h"
#include "world.h"
#include <SDL_ttf.h>


namespace Tmpl8 {


	class Surface;
	class Game
	{
	public:
		void SetTarget(Surface* surface, SDL_Renderer* renderer, SDL_Window* window, Audio* audio) { m_Screen = surface, m_Renderer = renderer, m_Window = window, m_Audio = audio; }
		void Init();
		void Shutdown();
		void Restart();

		void Tick(float deltaTime); // Main game function

		// Updates
		void Update(float deltaTime);
		void UpdateCameraY();
		void UpdateTimer(float deltaTime);
		/// Renders
		void Render(float deltaTime);
		void DrawAll();
		void Draw(Object* o);
		void DrawStatic(const Object& o);
		void RenderText(const char* text, int x, int y, SDL_Color color);

		// Player functions
		void ResetPlayer() { m_Player->resetPlayer(m_PlayerStartPos); }
		bool IsPlayerFinished();
		void CheckPlayerOutOfScreen();

		// Menu getters and setters
		GameMenu* GetMenu() { return m_Menu.get(); } // .get() returns a pointer to the GameMenu object because its a smart pointer.
		GameState GetState() const { return m_State; }
		void SetState(GameState state) { m_State = state; }


		// Send keys to the player class
		void KeyUp(int key) { m_Player->KeyUp(key); }
		void KeyDown(int key) { m_Player->KeyDown(key); if (DEVELOPER_MODE) m_Player->DevKeyDown(key, m_TileMap.get()); } // Overloaded function for when in developer mode
	private:
		Surface* m_Screen;
		SDL_Renderer* m_Renderer;
		SDL_Window* m_Window;
		Audio* m_Audio;
		TTF_Font* m_Font = nullptr; // Font for rendering text;
		std::unique_ptr<World> m_TileMap;
		std::unique_ptr<Player> m_Player;
		std::unique_ptr<GameMenu> m_Menu;
		Object m_Background;
		GameState m_State = GameState::Paused;
		vec2 m_PlayerStartPos;
		float m_CameraX, m_CameraY;
		float m_PlayerTimer = 0.0f;
		bool m_TimerActive = false;
		bool m_PlayerFinished = false;
	};

} // namespace Tmpl8
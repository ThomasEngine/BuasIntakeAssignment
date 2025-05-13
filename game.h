#pragma once
#include "object.h"
#include "entity.h"
#include <vector>
#include "world.h"
#include "player.h"



namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget(Surface* surface, SDL_Renderer* renderer, SDL_Window* window) { m_screen = surface, m_renderer = renderer, m_window = window; }
	void Init();
	void Shutdown();

	void Tick( float deltaTime );
	void Update(float deltaTime);
	void Render(float deltaTime);
	void showFPS(float deltaTime);
	void DrawAll();
	void Draw(Object o);

	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
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
	int Mapx, Mapy;
};

}; // namespace Tmpl8
#pragma once
#include "object.h"
#include "entity.h"
#include <vector>

constexpr int TILESIZE = 32;

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

	void loadMap(std::string filename);
	void drawMap();



	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown( int key ) { /* implement if you want to handle keys */ }
private:
	Surface* m_screen;
	SDL_Renderer* m_renderer;
	SDL_Window* m_window;
	Entity player;
	Object Background;
	int idol, walking;
	std::vector<Object> m_map;
};

}; // namespace Tmpl8
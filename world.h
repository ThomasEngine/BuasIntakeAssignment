#pragma once
#include <vector>
#include "object.h"
#include <SDL.h>

namespace Tmpl8
{

    class World
    {
    public:
        World(SDL_Renderer* renderer, int rows, int cols, int tileSize);
        ~World();

        void BuildTileMap(); // Pass the tilemap array
        void DrawTileMap(SDL_Renderer* renderer, int cameraX, int cameraY, int screenWidth, int screenHeight);

		const std::vector<Object>& GetMap() const { return m_map; } // Getter for m_map
    private:
        std::vector<Object> m_map;
        SDL_Renderer* m_renderer;
        //int m_rows, m_cols, TILESIZE;
    };
}
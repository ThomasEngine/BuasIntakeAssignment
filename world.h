#pragma once
#include <vector>
#include <string>
#include "object.h"
//#include "player.h"
#include <SDL.h>

namespace Tmpl8
{
    class World
    {
    public:
        World(SDL_Renderer* renderer, int tileSize);
        ~World();

        void LoadTilemapFromFile(const std::string& filename);
        void BuildTileMap();
        void DrawTileMap(SDL_Renderer* renderer, int cameraX, int cameraY, int screenWidth, int screenHeight, float player_y_change);

        const std::vector<Object>& GetMap() const { return m_map; }
        int GetRows() const { return m_rows; }
    private:
        std::vector<Object> m_map;
        SDL_Renderer* m_renderer;

        std::vector<std::vector<int>> m_tilemap;
        int m_rows, m_cols, m_tileSize;
    };
}
#pragma once
#include <vector>
#include <string>
#include "object.h"
#include "entity.h"
#include <SDL.h>

#include <SDL_image.h>

namespace Tmpl8
{
    class World
    {
    public:
        World(SDL_Renderer* renderer, int tileSize);
        ~World();

        void LoadTilemapFromFile(const std::string& filename);
        void BuildTileMap();
        void DrawTileMap(SDL_Renderer* renderer, int cameraX, int cameraY, int screenWidth, int screenHeight);

        void UpdateCoinAnimation();

        const std::vector<Object>& GetMap() const { return m_map; }
		std::vector<Entity>& GetCoins() { return m_coins; } 
        int GetRows() const { return m_rows; }
    private:
        std::vector<Object> m_map;
        SDL_Renderer* m_renderer;
        SDL_Texture* m_tileTexture;
        SDL_Texture* m_coinTexture;

        std::vector<std::vector<int>> m_tilemap;
        std::vector<Entity> m_coins;
        
        int m_rows, m_cols, m_tileSize;
    };
}
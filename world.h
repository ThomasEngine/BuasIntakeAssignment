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
        void DrawTileMap(SDL_Renderer* renderer, int m_CameraX, int m_CameraY, int screenWidth, int screenHeight);

        void UpdateCoinAnimation();

        const std::vector<Object>& GetMap() const { return m_Map; }
		std::vector<Entity>& GetCoins() { return m_coins; } 
        int GetRows() const { return m_Rows; }
    private:
        std::vector<Object> m_Map;
        SDL_Renderer* m_Renderer;
        SDL_Texture* m_TileTexture;
        SDL_Texture* m_CoinTexture;
        SDL_Texture* m_FinishFlagTexture;

        Object m_Flag;

        std::vector<std::vector<int>> m_tilemap;
        std::vector<Entity> m_coins;
        
        int m_Rows, m_Cols, m_TileSize;
    };
}
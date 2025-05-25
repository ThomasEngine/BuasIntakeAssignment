#pragma once
#include <vector>
#include <string>
#include "object.h"
#include "entity.h"
#include <SDL.h>

#include <SDL_image.h>
#include "template.h"

namespace Tmpl8
{
    class World
    {
    public:
        World(SDL_Renderer* renderer, int tileSize);
        ~World();
        // Init these once per level
        void LoadTilemapFromFile(const std::string& filename);
        void BuildTileMap();

        // Every frame renders and updates
        void DrawTileMap(SDL_Renderer* renderer, int m_CameraX, int m_CameraY, int screenWidth, int screenHeight);
        void UpdateCoinAnimation();

        // Getters
        const std::vector<Object>& GetMap() const { return m_Map; }
		std::vector<Entity>& GetCoins() { return m_coins; } 
        int GetRows() const { return m_Rows; }
        SDL_Rect GetFlagRect() const { return m_Flag.GetDest(); }
        vec2 GetPlayerStartingPos() const { return m_PlayerStart; }
    private:
        std::vector<Object> m_Map;
        SDL_Renderer* m_Renderer;
        SDL_Texture* m_TileTexture;
        SDL_Texture* m_CoinTexture;
        SDL_Texture* m_FinishFlagTexture;

        Object m_Flag;
        vec2 m_PlayerStart;

        std::vector<std::vector<int>> m_tilemap;
        std::vector<Entity> m_coins;
        
        int m_Rows, m_Cols, m_TileSize;
        enum TileTypeEnum
        {
            Coin = 99,
            FinishFlag = 88,
            Player = 77
        };
    };
}
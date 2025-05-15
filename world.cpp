#include "world.h"
#include <fstream>
#include <sstream>
#include <string>


namespace Tmpl8
{
    World::World(SDL_Renderer* renderer, int tileSize)
		: m_Renderer{ renderer }, m_TileSize{ tileSize }, m_TileTexture{nullptr}
    {
        // Load the tile texture
		SDL_Surface* surf = IMG_Load("assets/tilemap/Assets2.png");
        if (!surf)
        {
			SDL_Log("Failed to load tile texture: %s\n", IMG_GetError());
        }
		m_TileTexture = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);

		// Load the coin texture
		surf = IMG_Load("assets/coin.png");
        if (!surf)
        {
            SDL_Log("Failed to load coin texture: %s\n", IMG_GetError());
        }
		m_CoinTexture = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);

        // Load finish flag
        surf = IMG_Load("assets/finish.png");
        if (!surf)
        {
            SDL_Log("Failed to load coin texture: %s\n", IMG_GetError());
        }
        m_FinishFlagTexture = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);

    }

    World::~World()
    {
		SDL_DestroyTexture(m_TileTexture);
		SDL_DestroyTexture(m_CoinTexture);
        SDL_DestroyTexture(m_FinishFlagTexture);
        m_Map.clear();
        m_coins.clear();
    }

    void World::LoadTilemapFromFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        // Read the first line for rows and cols
        std::string firstLine;
        if (!std::getline(file, firstLine)) return;
        std::istringstream dimStream(firstLine);
        dimStream >> m_Rows >> m_Cols;

        // Prepare the tilemap
        m_tilemap.clear();
        m_tilemap.resize(m_Rows, std::vector<int>(m_Cols, 0));

        // Read the tile data
        std::string line;
        int row = 0;
        while (std::getline(file, line) && row < m_Rows)
        {
            std::istringstream iss(line);
            for (int col = 0; col < m_Cols; ++col)
            {
                int value;
                if (iss >> value)
                    m_tilemap[row][col] = value;
                else
                    m_tilemap[row][col] = 0; // Default if not enough values
            }
            ++row;
        }
        file.close();
    }

    void World::BuildTileMap()
    {
        m_Map.clear();
        m_coins.clear();

        Object tile;
		tile.SetTexture(m_TileTexture);

        for (int i = 0; i < m_Rows; i++)
        {
            for (int j = 0; j < m_Cols; j++)
            {
                int tileType = m_tilemap[i][j];

                if (tileType != 0)
                {
                    if (tileType == 99) // Coin
                    {

                        Entity coin;
                        coin.SetTexture(m_CoinTexture);
                        coin.SetDest(j * m_TileSize, i * m_TileSize, m_TileSize, m_TileSize);
                        coin.setCurAnimation(coin.createCycle(0, 16, 16, 16, 4));
                        m_coins.push_back(coin);
						continue;
                    }
                    if (tileType == 88) // Finish Flag
                    {
                        m_Flag.SetTexture(m_FinishFlagTexture);
                        m_Flag.SetSource(0, 0, 64, 64);
                        m_Flag.SetDest(j * m_TileSize, i * m_TileSize, m_TileSize * 2, m_TileSize * 2);
                        continue;
                    }
                    switch (tileType) // All the different tiles from tileset
                    {
					case 1:
						tile.SetSource(2 * m_TileSize, 0, m_TileSize, m_TileSize);
						break;
                    case 2:
                        tile.SetSource(3 * m_TileSize, 0, m_TileSize, m_TileSize);
						break;
					case 3:
						tile.SetSource(4 * m_TileSize, 0, m_TileSize, m_TileSize);
                        break;
					case 4:
						tile.SetSource(1 * m_TileSize, m_TileSize, m_TileSize, m_TileSize);
                        break;
					case 5:
						tile.SetSource(2 * m_TileSize, m_TileSize, m_TileSize, m_TileSize);
                        break;
					case 6:
						tile.SetSource(3 * m_TileSize, m_TileSize, m_TileSize, m_TileSize);
                        break;
					case 7:
						tile.SetSource(4 * m_TileSize, m_TileSize, m_TileSize, m_TileSize);
                        break;
					case 8:
						tile.SetSource(5 * m_TileSize, m_TileSize, m_TileSize, m_TileSize);
                        break;
					case 9:
						tile.SetSource(1 * m_TileSize, m_TileSize * 3, m_TileSize, m_TileSize);
						break;
					case 10:
						tile.SetSource(5 * m_TileSize, m_TileSize * 3, m_TileSize, m_TileSize);
                    default:
                        break;
                    }
                    tile.SetDest(j * m_TileSize, i * m_TileSize, m_TileSize, m_TileSize);
                    tile.SetSolid(true);
                    m_Map.push_back(tile);
                }
            }
        }
    }

    void World::DrawTileMap(SDL_Renderer* renderer, int m_CameraX, int m_CameraY, int screenWidth, int screenHeight)
    {
        for (Object& tile : m_Map)
        {
            // Only render tiles in view
            if (tile.GetDX() >= m_CameraX - m_TileSize &&
                tile.GetDY() >= m_CameraY - m_TileSize &&
                tile.GetDX() <= m_CameraX + screenWidth &&
                tile.GetDY() <= m_CameraY + screenHeight)
            {
                SDL_Rect dest = tile.GetDest();
                SDL_Rect src = tile.GetSource();

                dest.y -= m_CameraY;

                SDL_RenderCopy(renderer, tile.GetTex(), &src, &dest);
            }
        }
        for (Entity& coin : m_coins)
        {
            if (coin.GetDX() >= m_CameraX - m_TileSize &&
                coin.GetDY() >= m_CameraY - m_TileSize &&
                coin.GetDX() <= m_CameraX + screenWidth &&
                coin.GetDY() <= m_CameraY + screenHeight)
            {
                SDL_Rect dest = coin.GetDest();
                SDL_Rect src = coin.GetSource();
                dest.y -= m_CameraY;
                SDL_RenderCopy(renderer, coin.GetTex(), &src, &dest);
            }
        }
        if (m_Flag.GetDX() >= m_CameraX - m_TileSize &&
            m_Flag.GetDY() >= m_CameraY - m_TileSize &&
            m_Flag.GetDX() <= m_CameraX + screenWidth &&
            m_Flag.GetDY() <= m_CameraY + screenHeight)
        {
            SDL_Rect dest = m_Flag.GetDest();
            SDL_Rect src = m_Flag.GetSource();
            dest.y -= m_CameraY;
            SDL_RenderCopy(renderer, m_Flag.GetTex(), &src, &dest);
        }
        
    }
    void World::UpdateCoinAnimation()
    {
        for (Entity& coin : m_coins)
        {
            coin.updateAnimation();
        }
    }
}
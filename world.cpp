#include "world.h"
#include <fstream>
#include <sstream>
#include <string>


namespace Tmpl8
{
    World::World(SDL_Renderer* renderer, int tileSize)
		: m_Renderer{ renderer }, m_tileSize{ tileSize }, m_tileTexture{nullptr}
    {
        // Load the tile texture
		SDL_Surface* surf = IMG_Load("assets/tilemap/Assets2.png");
        if (!surf)
        {
			SDL_Log("Failed to load tile texture: %s\n", IMG_GetError());
        }
		m_tileTexture = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);

		// Load the coin texture
		surf = IMG_Load("assets/coin.png");
        if (!surf)
        {
            SDL_Log("Failed to load coin texture: %s\n", IMG_GetError());
        }
		m_coinTexture = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);

        // Load finish flag
        surf = IMG_Load("assets/finish.png");
        if (!surf)
        {
            SDL_Log("Failed to load coin texture: %s\n", IMG_GetError());
        }
        m_finishFlagTexture = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);

    }

    World::~World()
    {
		SDL_DestroyTexture(m_tileTexture);
		SDL_DestroyTexture(m_coinTexture);
        SDL_DestroyTexture(m_finishFlagTexture);
        m_map.clear();
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
        dimStream >> m_rows >> m_cols;

        // Prepare the tilemap
        m_tilemap.clear();
        m_tilemap.resize(m_rows, std::vector<int>(m_cols, 0));

        // Read the tile data
        std::string line;
        int row = 0;
        while (std::getline(file, line) && row < m_rows)
        {
            std::istringstream iss(line);
            for (int col = 0; col < m_cols; ++col)
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
        m_map.clear();
        m_coins.clear();

        Object tile;
		tile.SetTexture(m_tileTexture);

        for (int i = 0; i < m_rows; i++)
        {
            for (int j = 0; j < m_cols; j++)
            {
                int tileType = m_tilemap[i][j];

                if (tileType != 0)
                {
                    if (tileType == 99)
                    {

                        Entity coin;
                        coin.SetTexture(m_coinTexture);
                        coin.SetDest(j * m_tileSize, i * m_tileSize, m_tileSize, m_tileSize);
                        coin.setCurAnimation(coin.createCycle(0, 16, 16, 16, 4));
                        m_coins.push_back(coin);
						continue;
                    }
                    if (tileType == 88)
                    {
                        m_flag.SetTexture(m_finishFlagTexture);
                        m_flag.SetSource(0, 0, 64, 64);
                        m_flag.SetDest(j * m_tileSize, i * m_tileSize, m_tileSize * 2, m_tileSize * 2);
                        continue;
                    }
                    switch (tileType)
                    {
					case 1:
						tile.SetSource(2 * m_tileSize, 0, m_tileSize, m_tileSize);
						break;
                    case 2:
                        tile.SetSource(3 * m_tileSize, 0, m_tileSize, m_tileSize);
						break;
					case 3:
						tile.SetSource(4 * m_tileSize, 0, m_tileSize, m_tileSize);
                        break;
					case 4:
						tile.SetSource(1 * m_tileSize, m_tileSize, m_tileSize, m_tileSize);
                        break;
					case 5:
						tile.SetSource(2 * m_tileSize, m_tileSize, m_tileSize, m_tileSize);
                        break;
					case 6:
						tile.SetSource(3 * m_tileSize, m_tileSize, m_tileSize, m_tileSize);
                        break;
					case 7:
						tile.SetSource(4 * m_tileSize, m_tileSize, m_tileSize, m_tileSize);
                        break;
					case 8:
						tile.SetSource(5 * m_tileSize, m_tileSize, m_tileSize, m_tileSize);
                        break;
					case 9:
						tile.SetSource(1 * m_tileSize, m_tileSize * 3, m_tileSize, m_tileSize);
						break;
					case 10:
						tile.SetSource(5 * m_tileSize, m_tileSize * 3, m_tileSize, m_tileSize);
                    default:
                        break;
                    }
                    tile.SetDest(j * m_tileSize, i * m_tileSize, m_tileSize, m_tileSize);
                    tile.SetSolid(true);
                    m_map.push_back(tile);
                }
            }
        }
    }

    void World::DrawTileMap(SDL_Renderer* renderer, int m_CameraX, int m_CameraY, int screenWidth, int screenHeight)
    {
        for (Object& tile : m_map)
        {
            // Only render tiles in view (optional optimization)
            if (tile.GetDX() >= m_CameraX - m_tileSize &&
                tile.GetDY() >= m_CameraY - m_tileSize &&
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
            if (coin.GetDX() >= m_CameraX - m_tileSize &&
                coin.GetDY() >= m_CameraY - m_tileSize &&
                coin.GetDX() <= m_CameraX + screenWidth &&
                coin.GetDY() <= m_CameraY + screenHeight)
            {
                SDL_Rect dest = coin.GetDest();
                SDL_Rect src = coin.GetSource();
                dest.y -= m_CameraY;
                SDL_RenderCopy(renderer, coin.GetTex(), &src, &dest);
            }
        }
        if (m_flag.GetDX() >= m_CameraX - m_tileSize &&
            m_flag.GetDY() >= m_CameraY - m_tileSize &&
            m_flag.GetDX() <= m_CameraX + screenWidth &&
            m_flag.GetDY() <= m_CameraY + screenHeight)
        {
            SDL_Rect dest = m_flag.GetDest();
            SDL_Rect src = m_flag.GetSource();
            dest.y -= m_CameraY;
            SDL_RenderCopy(renderer, m_flag.GetTex(), &src, &dest);
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
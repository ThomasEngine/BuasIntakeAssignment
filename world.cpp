#include "world.h"
#include <fstream>
#include <sstream>
#include <string>


namespace Tmpl8
{
    World::World(SDL_Renderer* renderer, int tileSize)
		: m_renderer{ renderer }, m_tileSize{ tileSize }, m_rows{ 100 }, m_cols{ 40 }
    {
    }

    World::~World()
    {
        for (auto& tile : m_map)
        {
            SDL_DestroyTexture(tile.GetTex());
        }
        m_map.clear();
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

        Object tile;
        tile.SetImage("assets/tilemap/Assets2.png", m_renderer, 1);

        for (int i = 0; i < m_rows; i++)
        {
            for (int j = 0; j < m_cols; j++)
            {
                int tileType = m_tilemap[i][j];
                if (tileType != 0)
                {
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

    void World::DrawTileMap(SDL_Renderer* renderer, int cameraX, int cameraY, int screenWidth, int screenHeight /*Player* player*/)
    {
        for (const auto& tile : m_map)
        {
            int screenX = tile.GetDX() - cameraX;
            int screenY = tile.GetDY() - cameraY;
            SDL_Rect dest = tile.GetDest();
            dest.x = screenX;
            dest.y = screenY + cameraY/* + player->GetChangeY()*/;
            SDL_Rect src = tile.GetSource();
            SDL_RenderCopy(renderer, tile.GetTex(), &src, &dest);
             // tile.DrawRect(dest, m_renderer); // Uncomment if you want to draw rects
        }
    }
}
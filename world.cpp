#include "world.h"
#include "levels.h"

namespace Tmpl8
{
	World::World(SDL_Renderer* renderer, int rows, int cols, int tileSize)
		: m_renderer{renderer}
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

	void World::BuildTileMap()
	{
		m_map.clear(); // Clear any existing tiles

		Object tile;
		tile.SetImage("assets/tilemap/Assets2.png", m_renderer, 1); // Set the tile texture

		for (int i = 0; i < TILEMAP_ROWS; i++) // Loop through rows
		{
			for (int j = 0; j < TILEMAP_COLS; j++) // Loop through columns
			{
				int tileType = tilemap[i][j]; // Get the tile type from the tilemap

				if (tileType != 0) // Skip empty tiles
				{

					switch (tileType)
					{
					case 1:
						// Set the source rectangle based on the tile type
						tile.SetSource((tileType + 2) * TILESIZE, 0, TILESIZE, TILESIZE);
						break;
					case 2:
						tile.SetSource((tileType + 1) * TILESIZE, TILESIZE, TILESIZE, TILESIZE);
						break;
					default:
						break;
					}

					// Set the destination rectangle based on the tile's position in the grid
					tile.SetDest(j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE);
					tile.SetSolid(true); // Set the tile as solid

					// Add the tile to the map
					m_map.push_back(tile);
				}
			}
		}
	}

	void World::DrawTileMap(SDL_Renderer* renderer, int cameraX, int cameraY, int screenWidth, int screenHeight)
	{
		for (const auto& tile : m_map)
		{
			if (tile.GetDX() >= cameraX - TILESIZE &&
				tile.GetDY() >= cameraY - TILESIZE &&
				tile.GetDX() <= cameraX + screenWidth + TILESIZE &&
				tile.GetDY() <= cameraY + screenHeight + TILESIZE)
			{
				SDL_Rect dest = tile.GetDest();
				SDL_Rect src = tile.GetSource();
				SDL_RenderCopy(renderer, tile.GetTex(), &src, &dest);
			}
		}
	}

};
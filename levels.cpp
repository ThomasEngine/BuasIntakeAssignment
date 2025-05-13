#include "levels.h"

namespace Tmpl8 {
    int TILEMAP_ROWS = 40; // or your desired size
    int TILEMAP_COLS = 40; // or your desired size
    int TILESIZE = 32;

    std::vector<std::vector<int>> tilemap(TILEMAP_ROWS, std::vector<int>(TILEMAP_COLS, 0)
    );
}
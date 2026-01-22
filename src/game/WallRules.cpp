#include "game/WallRules.hpp"

namespace Game
{
    bool isWallPlacementLegalLocal(const uint8_t hWalls[WALL_GRID][WALL_GRID],
                                   const uint8_t vWalls[WALL_GRID][WALL_GRID],
                                   int x, int y, Orientation orientation)
    {
        if (x < 0 || x >= WALL_GRID || y < 0 || y >= WALL_GRID)
            return false;

        if (orientation == Orientation::Horizontal)
        {
            if (hWalls[x][y])
                return false;
            if (vWalls[x][y])
                return false;
            if (x > 0 && hWalls[x - 1][y])
                return false;
            if (x + 1 < WALL_GRID && hWalls[x + 1][y])
                return false;
        }
        else
        {
            if (vWalls[x][y])
                return false;
            if (hWalls[x][y])
                return false;
            if (y > 0 && vWalls[x][y - 1])
                return false;
            if (y + 1 < WALL_GRID && vWalls[x][y + 1])
                return false;
        }

        return true;
    }
}

#pragma once
#include "game/MoveTypes.hpp"
#include <cstdint>

namespace Game
{
    constexpr int WALL_GRID = 8;

    bool isWallPlacementLegalLocal(const uint8_t hWalls[WALL_GRID][WALL_GRID],
                                   const uint8_t vWalls[WALL_GRID][WALL_GRID],
                                   int x, int y, Orientation orientation);
}

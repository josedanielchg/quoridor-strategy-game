#pragma once
#include <cstdint>

namespace Game
{
    struct GameState
    {
        static constexpr int BOARD_SIZE = 9;
        static constexpr int WALL_GRID = 8;
        static constexpr int MAX_WALLS_PER_PLAYER = 10;

        uint8_t pawnX[2];
        uint8_t pawnY[2];
        uint8_t wallsRemaining[2];
        // Wall anchors (8x8). Horizontal blocks vertical edges, vertical blocks horizontal edges.
        uint8_t hWalls[WALL_GRID][WALL_GRID];
        uint8_t vWalls[WALL_GRID][WALL_GRID];
        // Distance to target row for each player; -1 means unreachable.
        int16_t distToGoal[2][BOARD_SIZE][BOARD_SIZE];
        // 1 means distToGoal must be recomputed (only walls affect it).
        uint8_t distDirty;
        uint8_t currentPlayerId;
        uint8_t winnerId;
    };
}

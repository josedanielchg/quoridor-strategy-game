#pragma once
#include <cstdint>

namespace Game
{
    struct GameState // Snapshot of game state #
    {
        static constexpr int BOARD_SIZE = 9;
        static constexpr int WALL_GRID = 8;
        static constexpr int MAX_WALLS_PER_PLAYER = 10;

        uint8_t pawnX[2]; // Pawn x positions #
        uint8_t pawnY[2]; // Pawn y positions #
        uint8_t wallsRemaining[2]; // Walls remaining per player #
        uint8_t hWalls[WALL_GRID][WALL_GRID]; // Horizontal wall anchors #
        uint8_t vWalls[WALL_GRID][WALL_GRID]; // Vertical wall anchors #
        int16_t distToGoal[2][BOARD_SIZE][BOARD_SIZE]; // Distance grid per player #
        uint8_t distDirty; // Distance cache dirty flag #
        uint8_t currentPlayerId; // Active player id #
        uint8_t winnerId; // Winner id or 0 #
    };
}

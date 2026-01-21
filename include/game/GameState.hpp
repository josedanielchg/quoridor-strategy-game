#pragma once
#include "game/Move.hpp"
#include <cstdint>
#include <vector>

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

        int currentPlayer() const;
        int winner() const;
        bool isGameOver() const;

        bool applyMove(const Move &move);
        bool hasPlayerWon(int playerId) const;
        std::vector<Move> generateLegalMoves() const;
    };

    void initGameState(GameState &state);
    bool isPawnMoveValid(const GameState &state, int playerId, int targetX, int targetY);
    bool isWallPlacementValid(const GameState &state, int playerId, int x, int y, Orientation orientation);
    bool hasPlayerWon(const GameState &state, int playerId);
    void computeDistancesToGoal(const GameState &state, int playerId,
                                int16_t dist[GameState::BOARD_SIZE][GameState::BOARD_SIZE]);
    void updateDistanceCache(GameState &state);
}

#pragma once
#include "game/GameState.hpp"
#include "game/Move.hpp"
#include <cstdint>
#include <vector>

namespace Game
{
    struct MoveUndoState
    {
        uint8_t pawnX;
        uint8_t pawnY;
        uint8_t wallsRemaining;
        uint8_t currentPlayerId;
        uint8_t winnerId;
        uint8_t distDirty;
    };

    void initGameState(GameState &state);

    int currentPlayer(const GameState &state);
    int winner(const GameState &state);
    bool isGameOver(const GameState &state);

    bool isPawnMoveValid(const GameState &state, int playerId, int targetX, int targetY);
    bool isWallPlacementValid(const GameState &state, int playerId, int x, int y, Orientation orientation);
    bool hasPlayerWon(const GameState &state, int playerId);
    bool applyMove(GameState &state, const Move &move);
    bool applyMoveUnchecked(GameState &state, const Move &move, MoveUndoState &undoState);
    void undoMove(GameState &state, const Move &move, const MoveUndoState &undoState);
    std::vector<Move> generateLegalMoves(const GameState &state);
    int evaluateState(GameState &state, int perspectivePlayerId);

    void computeDistancesToGoal(const GameState &state, int playerId,
                                int16_t dist[GameState::BOARD_SIZE][GameState::BOARD_SIZE]);
    void updateDistanceCache(GameState &state);
}

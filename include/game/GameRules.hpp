#pragma once
#include "game/GameState.hpp"
#include "game/Move.hpp"
#include <cstdint>
#include <vector>

namespace Game
{
    struct MoveUndoState // Temporary state for undoing moves #
    {
        uint8_t pawnX; // Previous pawn x #
        uint8_t pawnY; // Previous pawn y #
        uint8_t wallsRemaining; // Previous wall count #
        uint8_t currentPlayerId; // Previous turn id #
        uint8_t winnerId; // Previous winner id #
        uint8_t distDirty; // Previous distance dirty flag #
    };

    void initGameState(GameState &state); // Initialize a new game state #

    int currentPlayer(const GameState &state); // Get current player id #
    int winner(const GameState &state); // Get winner id #
    bool isGameOver(const GameState &state); // Check for game over #

    bool isPawnMoveValid(const GameState &state, int playerId, int targetX, int targetY); // Validate pawn move #
    bool isWallPlacementValid(const GameState &state, int playerId, int x, int y, Orientation orientation); // Validate wall placement #
    bool hasPlayerWon(const GameState &state, int playerId); // Check win condition #
    bool applyMove(GameState &state, const Move &move); // Apply a validated move #
    bool applyMoveUnchecked(GameState &state, const Move &move, MoveUndoState &undoState); // Apply without validation #
    void undoMove(GameState &state, const Move &move, const MoveUndoState &undoState); // Undo a move #
    std::vector<Move> generateLegalMoves(const GameState &state); // Generate legal moves #
    int evaluateState(GameState &state, int perspectivePlayerId); // Score a state #

    void computeDistancesToGoal(const GameState &state, int playerId,
                                int16_t dist[GameState::BOARD_SIZE][GameState::BOARD_SIZE]); // Compute distance grid #
    void updateDistanceCache(GameState &state); // Refresh cached distances #
}

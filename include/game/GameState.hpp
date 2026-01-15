#pragma once
#include "game/Board.hpp"
#include "game/Move.hpp"
#include <array>
#include <vector>

namespace Game
{
    struct PawnState
    {
        int x;
        int y;
    };

    struct WallState
    {
        int x;
        int y;
        Orientation orientation;
    };

    class GameState
    {
    public:
        static constexpr int MAX_WALLS_PER_PLAYER = 10;

    private:
        std::array<PawnState, 2> m_pawns;
        std::vector<WallState> m_walls;
        std::array<int, 2> m_wallsRemaining;
        int m_currentPlayer;
        int m_winner;

        bool buildBoard(Board &board) const;

    public:
        GameState();
        GameState(const Board &board, int currentPlayer, int p1WallsRemaining, int p2WallsRemaining);

        int currentPlayer() const;
        int wallsRemaining(int playerId) const;
        PawnState pawn(int playerId) const;
        const std::vector<WallState> &walls() const;
        int winner() const;
        bool isGameOver() const;

        bool applyMove(const Move &move);
        bool applyMoveUnchecked(const Move &move);
        bool hasPlayerWon(int playerId) const;
        std::vector<Move> generateLegalMoves() const;
        std::vector<Move> generatePawnMoves() const;
        bool syncBoard(Board &board) const;
    };
}

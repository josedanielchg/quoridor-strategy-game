#include "ai/AiEngine.hpp"
#include "ai/PathFinder.hpp"
#include <limits>

namespace Game
{
    int AiEngine::evaluate(const GameState &state, int maximizingPlayerId)
    {
        Board board;
        if (!state.syncBoard(board))
            return std::numeric_limits<int>::min();

        const PawnState p1 = state.pawn(1);
        const PawnState p2 = state.pawn(2);

        int distP1 = PathFinder::shortestPathLength(board, p1.x, p1.y, 0);
        int distP2 = PathFinder::shortestPathLength(board, p2.x, p2.y, Board::SIZE - 1);

        if (distP1 < 0)
            distP1 = 1000;
        if (distP2 < 0)
            distP2 = 1000;

        int wallDiff = state.wallsRemaining(1) - state.wallsRemaining(2);
        int score = (distP2 - distP1) * 10 + wallDiff;

        if (maximizingPlayerId == 2)
            score = -score;

        return score;
    }
}

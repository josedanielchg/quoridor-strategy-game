#pragma once
#include "game/GameState.hpp"

namespace Game
{
    class AiEngine
    {
    public:
        static int evaluate(const GameState &state, int maximizingPlayerId);
        static Move findBestMove(const GameState &state, int depth);

    private:
        static int negamax(GameState state, int depth, int alpha, int beta, int maximizingPlayerId);
    };
}

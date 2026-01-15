#pragma once
#include "game/GameState.hpp"

namespace Game
{
    class AiEngine
    {
    public:
        static int evaluate(const GameState &state, int maximizingPlayerId);
    };
}

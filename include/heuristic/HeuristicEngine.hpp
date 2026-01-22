#pragma once
#include "heuristic/TranspositionTable.hpp"
#include "game/GameState.hpp"
#include "game/Move.hpp"
#include <cstdint>

namespace Game
{
    enum class HeuristicDifficulty : uint8_t
    {
        Easy,
        Medium,
        Hard,
        Expert
    };

    struct HeuristicSearchConfig
    {
        int maxDepth = 3;
        int timeLimitMs = 4000;
        HeuristicDifficulty difficulty = HeuristicDifficulty::Medium;
        int weightDistance = 10;
        int weightWalls = 2;
        int weightWidth = 1;
    };

    class HeuristicEngine
    {
    public:
        explicit HeuristicEngine(HeuristicSearchConfig config = {});

        void setConfig(const HeuristicSearchConfig &config);
        const HeuristicSearchConfig &config() const;

        Move findBestMove(const GameState &state);

    private:
        HeuristicSearchConfig m_config;
        TranspositionTable m_tt;
    };
}

#pragma once
#include "heuristic/TranspositionTable.hpp"
#include "game/GameState.hpp"
#include "game/Move.hpp"
#include <cstdint>

namespace Game
{
    enum class HeuristicDifficulty : uint8_t // Difficulty levels #
    {
        Easy,
        Medium,
        Hard,
        Expert
    };

    struct HeuristicSearchConfig // Search tuning parameters #
    {
        int maxDepth = 4; // Max search depth #
        int timeLimitMs = 4000; // Time budget in ms #
        HeuristicDifficulty difficulty = HeuristicDifficulty::Medium; // Difficulty #
        int weightDistance = 10; // Distance weight #
        int weightWalls = 2; // Walls weight #
        int weightWidth = 1; // Width weight #
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

#pragma once
#include "ai/TranspositionTable.hpp"
#include "game/GameState.hpp"
#include "game/Move.hpp"
#include <cstdint>

namespace Game
{
    enum class AiDifficulty : uint8_t
    {
        Easy,
        Medium,
        Hard,
        Expert
    };

    struct AiSearchConfig
    {
        int maxDepth = 3;
        int timeLimitMs = 4000;
        AiDifficulty difficulty = AiDifficulty::Medium;
    };

    class AiEngine
    {
    public:
        explicit AiEngine(AiSearchConfig config = {});

        void setConfig(const AiSearchConfig &config);
        const AiSearchConfig &config() const;

        Move findBestMove(const GameState &state);

    private:
        AiSearchConfig m_config;
        TranspositionTable m_tt;
    };
}

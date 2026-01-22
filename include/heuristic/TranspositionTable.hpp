#pragma once
#include "game/GameState.hpp"
#include "game/Move.hpp"
#include <cstdint>
#include <optional>
#include <vector>

namespace Game
{
    enum class TTFlag : uint8_t
    {
        Exact,
        LowerBound,
        UpperBound
    };

    struct TTEntry
    {
        uint64_t key = 0;
        int16_t depth = -1;
        int32_t value = 0;
        TTFlag flag = TTFlag::Exact;
        std::optional<Move> bestMove;
        bool valid = false;
    };

    class TranspositionTable
    {
    public:
        explicit TranspositionTable(size_t sizePowerOfTwo = 1 << 20);

        void clear();
        void store(uint64_t key, int depth, int value, TTFlag flag, const Move &bestMove);
        const TTEntry *probe(uint64_t key) const;

    private:
        std::vector<TTEntry> m_entries;
        size_t m_mask = 0;
    };

    // Zobrist hash over logical game state (ignores distToGoal/distDirty).
    uint64_t computeZobrist(const GameState &state);
}

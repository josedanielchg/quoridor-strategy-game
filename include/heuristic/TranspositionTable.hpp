#pragma once
#include "game/GameState.hpp"
#include "game/Move.hpp"
#include <cstdint>
#include <optional>
#include <vector>

namespace Game
{
    enum class TTFlag : uint8_t // Transposition bound type #
    {
        Exact,
        LowerBound,
        UpperBound
    };

    struct TTEntry // Cached search result #
    {
        uint64_t key = 0;
        int16_t depth = -1;
        int32_t value = 0;
        TTFlag flag = TTFlag::Exact;
        std::optional<Move> bestMove;
        bool valid = false;
    };

    class TranspositionTable // Fixed-size hash table #
    {
    public:
        explicit TranspositionTable(size_t sizePowerOfTwo = 1 << 20); // Create table #

        void clear(); // Clear cached entries #
        void store(uint64_t key, int depth, int value, TTFlag flag, const Move &bestMove); // Store entry #
        const TTEntry *probe(uint64_t key) const; // Lookup entry #

    private:
        std::vector<TTEntry> m_entries;
        size_t m_mask = 0;
    };

    uint64_t computeZobrist(const GameState &state); // Zobrist hash of state #
}

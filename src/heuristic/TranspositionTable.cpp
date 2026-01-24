#include "heuristic/TranspositionTable.hpp"
#include <algorithm>
#include <random>

namespace Game
{
    namespace
    {
        // Round up to the next power of two for table sizing. #
        size_t nextPow2(size_t value)
        {
            if (value == 0)
                return 1;
            size_t v = value - 1;
            v |= v >> 1;
            v |= v >> 2;
            v |= v >> 4;
            v |= v >> 8;
            v |= v >> 16;
            if (sizeof(size_t) >= 8)
                v |= v >> 32;
            return v + 1;
        }

        // Random tables for Zobrist hashing. #
        struct ZobristTables
        {
            uint64_t pawn[2][GameState::BOARD_SIZE][GameState::BOARD_SIZE];
            uint64_t hWall[GameState::WALL_GRID][GameState::WALL_GRID];
            uint64_t vWall[GameState::WALL_GRID][GameState::WALL_GRID];
            uint64_t wallsRemaining[2][GameState::MAX_WALLS_PER_PLAYER + 1];
            uint64_t currentPlayer[2];
            uint64_t winner[3];
        };

        // Initialize and return shared Zobrist tables. #
        ZobristTables &tables()
        {
            static ZobristTables table = []()
            {
                ZobristTables t{};
                std::mt19937_64 rng(0xC0FFEEULL);
                auto next = [&rng]()
                { return rng(); };

                for (int p = 0; p < 2; ++p)
                {
                    for (int y = 0; y < GameState::BOARD_SIZE; ++y)
                    {
                        for (int x = 0; x < GameState::BOARD_SIZE; ++x)
                        {
                            t.pawn[p][y][x] = next();
                        }
                    }
                }

                for (int y = 0; y < GameState::WALL_GRID; ++y)
                {
                    for (int x = 0; x < GameState::WALL_GRID; ++x)
                    {
                        t.hWall[x][y] = next();
                        t.vWall[x][y] = next();
                    }
                }

                for (int p = 0; p < 2; ++p)
                {
                    for (int w = 0; w <= GameState::MAX_WALLS_PER_PLAYER; ++w)
                    {
                        t.wallsRemaining[p][w] = next();
                    }
                }

                t.currentPlayer[0] = next();
                t.currentPlayer[1] = next();

                t.winner[0] = next();
                t.winner[1] = next();
                t.winner[2] = next();

                return t;
            }();

            return table;
        }
    }

    // Compute a Zobrist hash for the logical game state. #
    uint64_t computeZobrist(const GameState &state)
    {
        const ZobristTables &t = tables();
        uint64_t hash = 0;

        for (int p = 0; p < 2; ++p)
        {
            int x = state.pawnX[p];
            int y = state.pawnY[p];
            if (x >= 0 && x < GameState::BOARD_SIZE && y >= 0 && y < GameState::BOARD_SIZE)
            {
                hash ^= t.pawn[p][y][x];
            }

            int walls = state.wallsRemaining[p];
            if (walls >= 0 && walls <= GameState::MAX_WALLS_PER_PLAYER)
            {
                hash ^= t.wallsRemaining[p][walls];
            }
        }

        for (int y = 0; y < GameState::WALL_GRID; ++y)
        {
            for (int x = 0; x < GameState::WALL_GRID; ++x)
            {
                if (state.hWalls[x][y])
                    hash ^= t.hWall[x][y];
                if (state.vWalls[x][y])
                    hash ^= t.vWall[x][y];
            }
        }

        if (state.currentPlayerId == 1 || state.currentPlayerId == 2)
            hash ^= t.currentPlayer[state.currentPlayerId - 1];

        if (state.winnerId <= 2)
            hash ^= t.winner[state.winnerId];

        return hash;
    }

    // Create a table with size rounded to power-of-two. #
    TranspositionTable::TranspositionTable(size_t sizePowerOfTwo)
    {
        size_t size = nextPow2(sizePowerOfTwo);
        m_entries.resize(size);
        m_mask = size - 1;
    }

    // Clear all cached entries and best-move hints. #
    void TranspositionTable::clear()
    {
        for (auto &entry : m_entries)
        {
            entry.valid = false;
            entry.bestMove.reset();
        }
    }

    // Store an entry if it is deeper or replaces an empty slot. #
    void TranspositionTable::store(uint64_t key, int depth, int value, TTFlag flag, const Move &bestMove)
    {
        size_t idx = key & m_mask;
        TTEntry &entry = m_entries[idx];

        if (!entry.valid || depth >= entry.depth)
        {
            entry.key = key;
            entry.depth = static_cast<int16_t>(depth);
            entry.value = value;
            entry.flag = flag;
            entry.bestMove = bestMove;
            entry.valid = true;
        }
    }

    // Probe the table for a key and return a valid entry if found. #
    const TTEntry *TranspositionTable::probe(uint64_t key) const
    {
        const TTEntry &entry = m_entries[key & m_mask];
        if (entry.valid && entry.key == key)
            return &entry;
        return nullptr;
    }
}

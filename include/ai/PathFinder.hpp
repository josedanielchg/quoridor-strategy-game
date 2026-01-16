#pragma once

#include "game/Board.hpp"
#include <vector>

namespace Game
{
    class PathFinder
    {
    public:
        
        static bool doesPathExist(const Board& board, int startX, int startY, int targetRow);
        static int shortestPathLength(const Board& board, int startX, int startY, int targetRow);
        static bool doesPathExistBFS(const Board& board, int startX, int startY, int targetRow);

        struct PathStats
        {
            long long bfsCalls = 0;
            long long bfsMs = 0;
            long long astarCalls = 0;
            long long astarMs = 0;
        };

        static void resetStats();
        static PathStats getStats();
    };
}

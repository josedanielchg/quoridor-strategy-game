#pragma once

#include "game/Board.hpp"
#include <vector>

namespace Game
{
    class PathFinder
    {
    public:
        
        static bool doesPathExist(const Board& board, int startX, int startY, int targetRow);
    };
}
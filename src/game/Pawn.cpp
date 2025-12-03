#include "game/Pawn.hpp"
#include "game/Board.hpp" // Now we can include Board fully
#include <cmath>

namespace Game
{

    Pawn::Pawn(int id, int startX, int startY)
        : Entity(startX, startY), m_id(id)
    {
    }

    int Pawn::id() const { return m_id; }

    std::string Pawn::getTexturePath() const
    {
        if (m_id == 1)
            return "assets/textures/pawn_p1.png";
        return "assets/textures/pawn_p2.png";
    }

    bool Pawn::isValidMove(const Board &board, int targetX, int targetY) const
    {
        // 1. Check Bounds
        if (!board.isValid(targetX, targetY))
            return false;

        // 2. Check if target is occupied (Basic rule)
        if (board.getPawnAt(targetX, targetY) != nullptr)
            return false;

        // 3. Calculate distance
        int dx = targetX - m_x;
        int dy = targetY - m_y;

        // Only allow 1 step (Up/Down/Left/Right) - No diagonals
        if (std::abs(dx) + std::abs(dy) != 1)
            return false;

        // 4. Check for Walls (Graph Connectivity)
        const Field &currentField = board.getField(m_x, m_y);

        if (dx == 1)
            return currentField.hasPath(Direction::Right);
        if (dx == -1)
            return currentField.hasPath(Direction::Left);
        if (dy == 1)
            return currentField.hasPath(Direction::Down);
        if (dy == -1)
            return currentField.hasPath(Direction::Up);

        return false;
    }
}
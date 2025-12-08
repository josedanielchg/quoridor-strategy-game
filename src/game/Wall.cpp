#include "game/Wall.hpp"
#include "game/Board.hpp"

namespace Game
{

    Wall::Wall(int x, int y, Orientation orientation)
        : Entity(x, y), m_orientation(orientation) {}

    Orientation Wall::orientation() const { return m_orientation; }

    std::string Wall::getTexturePath() const
    {
        return "assets/textures/wall.png";
    }

    bool Wall::isValidMove(const Board &board, int targetX, int targetY) const
    {
        // 1. Check Bounds
        if (targetX < 0 || targetX >= Board::SIZE - 1 ||
            targetY < 0 || targetY >= Board::SIZE - 1)
        {
            return false;
        }

        // 2. Check Overlap with existing walls
        for (const auto &w : board.getAllWalls())
        {
            // Exact same position?
            if (w.x() == targetX && w.y() == targetY)
                return false;

            // TODO:
            // Vertical vs Horizontal Cross overlap?
            // (A vertical wall at x,y doesn't block a horizontal at x,y directly,
            // but usually in Quoridor they cannot cross center).
            // Simplification: Can't place if exact center is taken.
            // but we want strict Quoridor rules, walls cannot cross each other.
        }

        return true;
    }
}
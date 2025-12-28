#include "game/Wall.hpp"
#include "game/Board.hpp"

namespace Game
{

    Wall::Wall(int x, int y, Orientation orientation)
        : VisualEntity(x, y), m_orientation(orientation)
    {
        (void)initSprite();
    }

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

        // 2. Check overlap with existing walls
        for (const auto &w : board.getAllWalls())
        {
            // Reject perpendicular walls sharing the same anchor
            if (w.x() == targetX && w.y() == targetY && w.orientation() != m_orientation)
                return false;

            if (w.orientation() == m_orientation)
            {
                int diffX = std::abs(w.x() - targetX);
                int diffY = std::abs(w.y() - targetY);

                if (m_orientation == Orientation::Vertical)
                {
                    // Vertical walls cannot overlap or share a segment
                    if (diffX == 0 && diffY < 2) return false;
                }
                else // Horizontal
                {
                    // Horizontal walls cannot overlap or share a segment
                    if (diffY == 0 && diffX < 2) return false;
                }
            }
        }

        return true;
    }
}

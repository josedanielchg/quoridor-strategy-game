#include "game/Pawn.hpp"
#include "game/Board.hpp"
#include <cmath>

namespace Game
{

    Pawn::Pawn(int id, int startX, int startY)
        : VisualEntity(startX, startY), m_id(id)
    {
        (void)initSprite();
    }

    int Pawn::id() const { return m_id; }

    std::string Pawn::getTexturePath() const
    {
        if (m_id == 1)
            return "assets/textures/pawn_p1.png";
        return "assets/textures/pawn_p2.png";
    }

    sf::Vector2f Pawn::getSpriteOrigin(const sf::Vector2u &texSize) const
    {
        // Hardcoded values to preserve previous visual tweaks
        return {112.f / 2.f, 165.f - 20.f};
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
        int distance = std::abs(dx) + std::abs(dy);

        // Only allow 1 step or 2 step (jump) moves - No diagonals
        if (distance != 1 && distance != 2)
            return false;

        // Determine direction (normalized)
        int dirX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
        int dirY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

        // 4. Check for Walls (Graph Connectivity) for the first step
        const Field &currentField = board.getField(m_x, m_y);

        if (dirX == 1 && !currentField.hasPath(Direction::Right))
            return false;
        if (dirX == -1 && !currentField.hasPath(Direction::Left))
            return false;
        if (dirY == 1 && !currentField.hasPath(Direction::Down))
            return false;
        if (dirY == -1 && !currentField.hasPath(Direction::Up))
            return false;

        // If it's a normal 1-step move, we're done
        if (distance == 1)
            return true;

        // If it's a 2-step move (jump), validate jump logic
        if (distance == 2)
        {
            // Check if there's an opponent pawn at the intermediate position
            int intermediateX = m_x + dirX;
            int intermediateY = m_y + dirY;

            if (!board.isValid(intermediateX, intermediateY))
                return false;

            const Pawn *intermediatePawn = board.getPawnAt(intermediateX, intermediateY);
            if (intermediatePawn == nullptr || intermediatePawn->id() == m_id)
                return false; // No opponent to jump over

            // Check if we can move from intermediate to target (wall check)
            const Field &intermediateField = board.getField(intermediateX, intermediateY);

            if (dirX == 1 && !intermediateField.hasPath(Direction::Right))
                return false;
            if (dirX == -1 && !intermediateField.hasPath(Direction::Left))
                return false;
            if (dirY == 1 && !intermediateField.hasPath(Direction::Down))
                return false;
            if (dirY == -1 && !intermediateField.hasPath(Direction::Up))
                return false;

            return true;
        }

        return false;
    }
}
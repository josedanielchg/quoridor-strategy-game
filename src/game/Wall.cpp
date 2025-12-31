#include "game/Wall.hpp"
#include "game/Board.hpp"
#include <iostream>

namespace Game
{

    Wall::Wall(int x, int y, Orientation orientation)
        : VisualEntity(x, y),
          m_orientation(orientation),
          m_texDown(),
          m_spriteDown(m_texDown)
    {
        (void)initSprite();

        if (!m_texDown.loadFromFile("assets/textures/wall-down.png"))
        {
            std::cerr << "Error: Could not load wall-down.png\n";
        }
        else
        {
            m_spriteDown.setTexture(m_texDown, true);
            sf::Vector2u downSize = m_texDown.getSize();
            m_spriteDown.setOrigin({float(downSize.x) / 1.08f, float(downSize.y) / 1.31f});
        }
    }

    Wall::Wall(Wall &&other) noexcept
        : VisualEntity(std::move(other)),
          m_orientation(other.m_orientation),
          m_texDown(std::move(other.m_texDown)),
          m_spriteDown(std::move(other.m_spriteDown))
    {
        m_spriteDown.setTexture(m_texDown, false);
    }

    Wall &Wall::operator=(Wall &&other) noexcept
    {
        if (this != &other)
        {
            VisualEntity::operator=(std::move(other));
            m_orientation = other.m_orientation;
            m_texDown = std::move(other.m_texDown);
            m_spriteDown = std::move(other.m_spriteDown);
            m_spriteDown.setTexture(m_texDown, false);
        }
        return *this;
    }

    Orientation Wall::orientation() const { return m_orientation; }

    const sf::Sprite &Wall::upperSprite() const { return sprite(); }

    const sf::Sprite &Wall::downSprite() const { return m_spriteDown; }

    const Wall &Wall::previewWall()
    {
        static Wall wall(0, 0, Orientation::Horizontal);
        return wall;
    }

    const sf::Sprite &Wall::previewUpperSprite()
    {
        return previewWall().upperSprite();
    }

    const sf::Sprite &Wall::previewDownSprite()
    {
        return previewWall().downSprite();
    }

    std::string Wall::getTexturePath() const
    {
        return "assets/textures/wall-upper.png";
    }

    sf::Vector2f Wall::getSpriteOrigin(const sf::Vector2u &texSize) const
    {
        return {float(texSize.x) / 1.0f, float(texSize.y) / 1.31f};
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

#pragma once
#include "game/VisualEntity.hpp"

namespace Game
{

    enum class Orientation
    {
        Horizontal,
        Vertical
    };

    class Wall : public VisualEntity
    {
    private:
        Orientation m_orientation;
        sf::Texture m_texDown;
        sf::Sprite m_spriteDown;

        static const Wall &previewWall();

    public:
        Wall(int x, int y, Orientation orientation);
        Wall(Wall &&other) noexcept;
        Wall &operator=(Wall &&other) noexcept;

        Orientation orientation() const;
        const sf::Sprite &upperSprite() const;
        const sf::Sprite &downSprite() const;
        static const sf::Sprite &previewUpperSprite();
        static const sf::Sprite &previewDownSprite();

        // Checks if placing a wall here is valid (Overlap, Bounds)
        bool isValidMove(const Board &board, int targetX, int targetY) const override;

        std::string getTexturePath() const override;
        sf::Vector2f getSpriteOrigin(const sf::Vector2u &texSize) const override;
    };
}

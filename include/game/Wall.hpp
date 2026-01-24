#pragma once
#include "game/MoveTypes.hpp"
#include "game/VisualEntity.hpp"

namespace Game
{
    class Wall : public VisualEntity
    {
    private:
        Orientation m_orientation;
        sf::Texture m_texDown;
        sf::Sprite m_spriteDown;

        static const Wall &previewWall(); // Shared preview wall #

    public:
        Wall(int x, int y, Orientation orientation); // Create wall #
        Wall(Wall &&other) noexcept; // Move construct wall #
        Wall &operator=(Wall &&other) noexcept; // Move assign wall #

        Orientation orientation() const; // Get wall orientation #
        const sf::Sprite &upperSprite() const; // Upper wall sprite #
        const sf::Sprite &downSprite() const; // Down wall sprite #
        static const sf::Sprite &previewUpperSprite(); // Upper preview sprite #
        static const sf::Sprite &previewDownSprite(); // Down preview sprite #

        bool isValidMove(const Board &board, int targetX, int targetY) const override; // Validate placement #

        std::string getTexturePath() const override; // Wall texture path #
        sf::Vector2f getSpriteOrigin(const sf::Vector2u &texSize) const override; // Wall origin #
    };
}

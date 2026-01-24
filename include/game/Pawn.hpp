#pragma once
#include "game/VisualEntity.hpp"

namespace Game
{

    class Pawn : public VisualEntity
    {
    private:
        int m_id;

    public:
        Pawn(int id, int startX, int startY); // Create pawn with id #

        int id() const; // Get pawn id #

        bool isValidMove(const Board &board, int targetX, int targetY) const override; // Validate pawn move #
        std::string getTexturePath() const override; // Pawn texture path #

        sf::Vector2f getSpriteOrigin(const sf::Vector2u &texSize) const override; // Pawn sprite origin #
    };
}

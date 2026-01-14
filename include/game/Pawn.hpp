#pragma once
#include "game/VisualEntity.hpp"


namespace Game
{

    class Pawn : public VisualEntity
    {
    private:
        int m_id;

    public:
        Pawn(int id, int startX, int startY);

        int id() const;

        // Checks if the move to (x, y) is legal (adjacency + walls)
        bool isValidMove(const Board &board, int targetX, int targetY) const override;
        std::string getTexturePath() const override;

        // Optional: provide custom sprite origin for pawns
        sf::Vector2f getSpriteOrigin(const sf::Vector2u &texSize) const override;
    };
}
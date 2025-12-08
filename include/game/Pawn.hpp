#pragma once
#include "game/Entity.hpp"

namespace Game
{

    class Pawn : public Entity
    {
    private:
        int m_id;

    public:
        Pawn(int id, int startX, int startY);

        int id() const;

        // Checks if the move to (x, y) is legal (adjacency + walls)
        bool isValidMove(const Board &board, int targetX, int targetY) const override;

        std::string getTexturePath() const override;
    };
}
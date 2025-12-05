#pragma once
#include "game/Entity.hpp"

namespace Game
{

    enum class Orientation
    {
        Horizontal,
        Vertical
    };

    class Wall : public Entity
    {
    private:
        Orientation m_orientation;

    public:
        Wall(int x, int y, Orientation orientation);

        Orientation orientation() const;

        // Checks if placing a wall here is valid (Overlap, Bounds)
        bool isValidMove(const Board &board, int targetX, int targetY) const override;

        std::string getTexturePath() const override;
    };
}
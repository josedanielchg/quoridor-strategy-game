#pragma once
#include "game/MoveTypes.hpp"

namespace Game
{

    class Move
    {
    private:
        MoveType m_type;
        int m_x;
        int m_y;
        Orientation m_orientation;
        int m_playerId;

    public:
        Move(MoveType type, int x, int y, Orientation orientation, int playerId); // Build a move #

        static Move Pawn(int x, int y, int playerId); // Pawn move factory #
        static Move Wall(int x, int y, Orientation orientation, int playerId); // Wall move factory #

        MoveType type() const; // Get move type #
        int x() const; // Get x coordinate #
        int y() const; // Get y coordinate #
        Orientation orientation() const; // Get wall orientation #
        int playerId() const; // Get player id #
    };
}

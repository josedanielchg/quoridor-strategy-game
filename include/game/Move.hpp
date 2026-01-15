#pragma once
#include "game/Wall.hpp"

namespace Game
{
    enum class MoveType
    {
        PawnMove,
        WallPlacement
    };

    class Move
    {
    private:
        MoveType m_type;
        int m_x;
        int m_y;
        Orientation m_orientation;
        int m_playerId;

    public:
        Move(MoveType type, int x, int y, Orientation orientation, int playerId);

        static Move Pawn(int x, int y, int playerId);
        static Move Wall(int x, int y, Orientation orientation, int playerId);

        MoveType type() const;
        int x() const;
        int y() const;
        Orientation orientation() const;
        int playerId() const;
    };
}

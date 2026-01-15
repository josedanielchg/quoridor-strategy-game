#include "game/Move.hpp"

namespace Game
{
    Move::Move(MoveType type, int x, int y, Orientation orientation, int playerId)
        : m_type(type), m_x(x), m_y(y), m_orientation(orientation), m_playerId(playerId)
    {
    }

    Move Move::Pawn(int x, int y, int playerId)
    {
        return Move(MoveType::PawnMove, x, y, Orientation::Horizontal, playerId);
    }

    Move Move::Wall(int x, int y, Orientation orientation, int playerId)
    {
        return Move(MoveType::WallPlacement, x, y, orientation, playerId);
    }

    MoveType Move::type() const { return m_type; }
    int Move::x() const { return m_x; }
    int Move::y() const { return m_y; }
    Orientation Move::orientation() const { return m_orientation; }
    int Move::playerId() const { return m_playerId; }
}

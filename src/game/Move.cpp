#include "game/Move.hpp"

namespace Game
{
    // Construct a move with explicit parameters. #
    Move::Move(MoveType type, int x, int y, Orientation orientation, int playerId)
        : m_type(type), m_x(x), m_y(y), m_orientation(orientation), m_playerId(playerId)
    {
    }

    // Factory for pawn move. #
    Move Move::Pawn(int x, int y, int playerId)
    {
        return Move(MoveType::PawnMove, x, y, Orientation::Horizontal, playerId);
    }

    // Factory for wall placement move. #
    Move Move::Wall(int x, int y, Orientation orientation, int playerId)
    {
        return Move(MoveType::WallPlacement, x, y, orientation, playerId);
    }

    // Return the move type. #
    MoveType Move::type() const { return m_type; }
    // Return x coordinate. #
    int Move::x() const { return m_x; }
    // Return y coordinate. #
    int Move::y() const { return m_y; }
    // Return wall orientation. #
    Orientation Move::orientation() const { return m_orientation; }
    // Return player id. #
    int Move::playerId() const { return m_playerId; }
}

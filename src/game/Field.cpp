#include "game/Field.hpp"

namespace Game
{
    // Build a field at origin with all connections open. #
    Field::Field() : m_x(0), m_y(0), m_occupantId(0)
    {
        std::fill(std::begin(m_connections), std::end(m_connections), true);
    }

    // Build a field at coordinates with all connections open. #
    Field::Field(int x, int y) : m_x(x), m_y(y), m_occupantId(0)
    {
        std::fill(std::begin(m_connections), std::end(m_connections), true);
    }

    // Return field x coordinate. #
    int Field::x() const { return m_x; }
    // Return field y coordinate. #
    int Field::y() const { return m_y; }
    // Return current occupant id. #
    int Field::occupantId() const { return m_occupantId; }

    // Set the occupant id for this field. #
    void Field::setOccupantId(int id) { m_occupantId = id; }

    // Remove a connection in the given direction. #
    void Field::disconnect(Direction dir)
    {
        m_connections[static_cast<int>(dir)] = false;
    }
    // Restore a connection in the given direction. #
    void Field::connect(Direction dir)
    {
        m_connections[static_cast<int>(dir)] = true;
    }

    // Check if a connection exists in the given direction. #
    bool Field::hasPath(Direction dir) const
    {
        return m_connections[static_cast<int>(dir)];
    }
}

#include "game/Field.hpp"

namespace Game
{
    // Connections follow Direction enum order: Up, Right, Down, Left.
    Field::Field() : m_x(0), m_y(0), m_occupantId(0)
    {
        std::fill(std::begin(m_connections), std::end(m_connections), true);
    }

    Field::Field(int x, int y) : m_x(x), m_y(y), m_occupantId(0)
    {
        std::fill(std::begin(m_connections), std::end(m_connections), true);
    }

    // Getters
    int Field::x() const { return m_x; }
    int Field::y() const { return m_y; }
    int Field::occupantId() const { return m_occupantId; }

    // Setters
    void Field::setOccupantId(int id) { m_occupantId = id; }

    void Field::disconnect(Direction dir)
    {
        m_connections[static_cast<int>(dir)] = false;
    }
    void Field::connect(Direction dir)
    {
        m_connections[static_cast<int>(dir)] = true;
    }

    bool Field::hasPath(Direction dir) const
    {
        return m_connections[static_cast<int>(dir)];
    }
}

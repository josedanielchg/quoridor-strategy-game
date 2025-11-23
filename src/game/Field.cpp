#include "game/Field.hpp"

namespace Game
{

    Field::Field(int x, int y): m_x(x), m_y(y), m_occupantId(0)
    {
        // Reserve 4 to avoid reallocations, standard grid max neighbors
        m_neighbors.reserve(4);
    }

    // Getters
    int Field::x() const { return m_x; }
    int Field::y() const { return m_y; }
    int Field::occupantId() const { return m_occupantId; }

    // Setters
    void Field::setOccupantId(int id) { m_occupantId = id; }

    // ====
    
    void Field::addNeighbor(Field *neighbor)
    {
        if (neighbor && !hasNeighbor(neighbor))
        {
            m_neighbors.push_back(neighbor);
        }
    }

    void Field::removeNeighbor(Field *neighbor)
    {
        // C++20 Erase-Remove idiom simplified
        std::erase(m_neighbors, neighbor);
    }

    const std::vector<Field *> &Field::getNeighbors() const
    {
        return m_neighbors;
    }

    bool Field::hasNeighbor(const Field *neighbor) const
    {
        return std::find(m_neighbors.begin(), m_neighbors.end(), neighbor) != m_neighbors.end();
    }
}
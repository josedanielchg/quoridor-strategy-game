#include "game/Wall.hpp"

namespace Game
{

    Wall::Wall(int x, int y, Orientation orientation) : m_x(x), m_y(y), m_orientation(orientation) {}

    int Wall::x() const { return m_x; }
    int Wall::y() const { return m_y; }
    Orientation Wall::orientation() const { return m_orientation; }

}
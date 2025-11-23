#pragma once

namespace Game
{
    enum class Orientation
    {
        Horizontal,
        Vertical
    };

    class Wall
    {
        private:
            int m_x;
            int m_y;
            Orientation m_orientation;

        public:
            Wall(int x, int y, Orientation orientation);

            int x() const;
            int y() const;
            Orientation orientation() const;
    };

}
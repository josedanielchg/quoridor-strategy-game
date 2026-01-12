#pragma once
#include <vector>
#include <algorithm>

namespace Game
{
    enum class Direction { Up = 0, Right = 1, Down = 2, Left = 3, Count = 4 };

    class Field
    {
        private:
            int m_x;
            int m_y;
            int m_occupantId;

            bool m_connections[static_cast<int>(Direction::Count)];

        public:
            Field();
            Field(int x, int y);

            // Getters
            int x() const;
            int y() const;
            int occupantId() const; // 0 = None, 1 = P1, 2 = P2

            // Setters
            void setOccupantId(int id);

            // Graph Logic
            void disconnect(Direction dir);
            void connect(Direction dir);

            bool hasPath(Direction dir) const;
    };
}
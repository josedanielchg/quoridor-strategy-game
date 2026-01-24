#pragma once
#include <vector>
#include <algorithm>

namespace Game
{
    enum class Direction
    {
        Up = 0,
        Right = 1,
        Down = 2,
        Left = 3,
        Count = 4
    };

    class Field
    {
    private:
        int m_x;
        int m_y;
        int m_occupantId;

        bool m_connections[static_cast<int>(Direction::Count)];

    public:
        Field(); // Create empty field #
        Field(int x, int y); // Create field at coords #

        int x() const; // Get x position #
        int y() const; // Get y position #
        int occupantId() const; // Get occupant id #

        void setOccupantId(int id); // Set occupant id #

        void disconnect(Direction dir); // Remove edge #
        void connect(Direction dir); // Add edge #

        bool hasPath(Direction dir) const; // Check edge #
    };
}

#pragma once
#include <string>

namespace Game
{

    class Board;

    class Entity
    {
    protected:
        int m_x;
        int m_y;

    public:
        Entity(int x, int y) : m_x(x), m_y(y) {} // Initialize position #
        virtual ~Entity() = default; // Polymorphic cleanup #

        int x() const { return m_x; } // Get x position #
        int y() const { return m_y; } // Get y position #

        void setPosition(int x, int y) // Update entity position #
        {
            m_x = x;
            m_y = y;
        }

        virtual bool isValidMove(const Board &board, int targetX, int targetY) const = 0; // Validate a move #

        virtual std::string getTexturePath() const = 0; // Texture path for visuals #
    };
}

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
        Entity(int x, int y) : m_x(x), m_y(y) {}
        virtual ~Entity() = default;

        // Getters
        int x() const { return m_x; }
        int y() const { return m_y; }

        void setPosition(int x, int y)
        {
            m_x = x;
            m_y = y;
        }

        // MANDATORY Methods
        virtual bool isValidMove(const Board &board, int targetX, int targetY) const = 0;

        // Visuals
        virtual std::string getTexturePath() const = 0;
    };
}
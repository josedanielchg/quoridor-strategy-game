#include "game/Board.hpp"
#include <iostream>

namespace Game
{
    Board::Board()
    {
        // Pre-allocate fields
        m_fields.reserve(SIZE * SIZE);
        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
            {
                m_fields.emplace_back(x, y);
            }
        }
    }

    void Board::init()
    {
        // Reset neighbors
        // This creates the full graph where every cell connects to adjacent ones
        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
            {
                Field *current = getField(x, y);

                if (y > 0)
                    current->addNeighbor(getField(x, y - 1)); // Up
                if (y < SIZE - 1)
                    current->addNeighbor(getField(x, y + 1)); // Down
                if (x > 0)
                    current->addNeighbor(getField(x - 1, y)); // Left
                if (x < SIZE - 1)
                    current->addNeighbor(getField(x + 1, y)); // Right
            }
        }
        m_walls.clear();
    }

    Field *Board::getField(int x, int y)
    {
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
            return nullptr;
        return &m_fields[y * SIZE + x];
    }

    const Field *Board::getField(int x, int y) const
    {
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
            return nullptr;
        return &m_fields[y * SIZE + x];
    }

    const std::vector<Field> &Board::getAllFields() const
    {
        return m_fields;
    }

    const std::vector<Wall> &Board::getAllWalls() const
    {
        return m_walls;
    }

    void Board::disconnect(Field *a, Field *b)
    {
        if (a && b)
        {
            a->removeNeighbor(b);
            b->removeNeighbor(a);
        }
    }

    bool Board::placeWall(int x, int y, Orientation orientation)
    {
        // Validate bounds for a wall (walls go between cells)
        if (x < 0 || x >= SIZE - 1 || y < 0 || y >= SIZE - 1)
            return false;

        if (orientation == Orientation::Horizontal)
        {
            // Horizontal wall at (x,y) cuts:
            // (x,y)|(x,y+1) AND (x+1,y)|(x+1,y+1)
            Field *tl = getField(x, y);         // Top-Left
            Field *bl = getField(x, y + 1);     // Bottom-Left
            Field *tr = getField(x + 1, y);     // Top-Right
            Field *br = getField(x + 1, y + 1); // Bottom-Right

            disconnect(tl, bl);
            disconnect(tr, br);
        }
        else
        {
            // Vertical wall at (x,y) cuts:
            // (x,y)|(x+1,y) AND (x,y+1)|(x+1,y+1)
            Field *tl = getField(x, y);
            Field *tr = getField(x + 1, y);
            Field *bl = getField(x, y + 1);
            Field *br = getField(x + 1, y + 1);

            disconnect(tl, tr);
            disconnect(bl, br);
        }

        m_walls.emplace_back(x, y, orientation);
        return true;
    }
}
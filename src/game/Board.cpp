#include "game/Board.hpp"
#include <stdexcept>

namespace Game
{

    Board::Board()
    {
        init();
    }

    void Board::init()
    {
        m_fields.clear();
        m_fields.reserve(SIZE * SIZE);
        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
            {
                m_fields.emplace_back(x, y);
            }
        }
        m_walls.clear();

        // Initialize Players here instead of Application
        m_pawns.clear();
        m_pawns.emplace_back(1, 4, 8); // Player 1 starts bottom
        m_pawns.emplace_back(2, 4, 0); // Player 2 starts top
    }

    Field &Board::getField(int x, int y)
    {
        if (!isValid(x, y))
            throw std::out_of_range("Out of bounds");
        return m_fields[y * SIZE + x];
    }

    const Field &Board::getField(int x, int y) const
    {
        if (!isValid(x, y))
            throw std::out_of_range("Out of bounds");
        return m_fields[y * SIZE + x];
    }

    const std::vector<Field> &Board::getAllFields() const { return m_fields; }
    const std::vector<Wall> &Board::getAllWalls() const { return m_walls; }
    const std::vector<Pawn> &Board::getAllPawns() const { return m_pawns; }

    const Pawn *Board::getPawnAt(int x, int y) const
    {
        for (const auto &p : m_pawns)
        {
            if (p.x() == x && p.y() == y)
                return &p;
        }
        return nullptr;
    }

    Pawn *Board::getPawnById(int id)
    {
        for (auto &p : m_pawns)
        {
            if (p.id() == id)
                return &p;
        }
        return nullptr;
    }

    bool Board::placeWall(int x, int y, Orientation orientation)
    {
        // Create temp wall to check validity
        Wall tempWall(x, y, orientation);

        if (!tempWall.isValidMove(*this, x, y))
            return false;

        // Apply Logic (Cutting connections)
        if (orientation == Orientation::Horizontal)
        {
            getField(x, y).disconnect(Direction::Down);
            getField(x, y + 1).disconnect(Direction::Up);
            getField(x + 1, y).disconnect(Direction::Down);
            getField(x + 1, y + 1).disconnect(Direction::Up);
        }
        else
        {
            getField(x, y).disconnect(Direction::Right);
            getField(x + 1, y).disconnect(Direction::Left);
            getField(x, y + 1).disconnect(Direction::Right);
            getField(x + 1, y + 1).disconnect(Direction::Left);
        }

        m_walls.emplace_back(x, y, orientation);
        return true;
    }

    bool Board::movePawn(int pawnId, int targetX, int targetY)
    {
        Pawn *pawn = getPawnById(pawnId);
        if (!pawn)
            return false;

        // Ask the Pawn class if the move is valid
        if (pawn->isValidMove(*this, targetX, targetY))
        {
            pawn->setPosition(targetX, targetY);
            return true;
        }
        return false;
    }

    bool Board::isValid(int x, int y) const
    {
        return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
    }
}
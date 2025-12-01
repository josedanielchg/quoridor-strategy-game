#include "game/Board.hpp"
#include <stdexcept>

namespace Game
{
    Board::Board()
    {
        // El std::array ya está creado en memoria. Solo configuramos las coordenadas.
        m_fields.reserve(SIZE * SIZE);
        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
            {
                // Usamos emplace_back como antes, que funciona perfecto con vector
                m_fields.emplace_back(x, y);
            }
        }
    }

    void Board::init()
    {
        // Reiniciar el tablero sobrescribiendo con Fields nuevos (conexiones en true)
        m_fields.clear();
        m_fields.reserve(SIZE * SIZE);
        for (int y = 0; y < SIZE; ++y) {
            for (int x = 0; x < SIZE; ++x) {
                m_fields.emplace_back(x, y); // Crea Fields nuevos conectados
            }
        }
        m_walls.clear();
    }

    Field& Board::getField(int x, int y)
    {
        if (!isValid(x, y)) throw std::out_of_range("Out of bounds");
        return m_fields[y * SIZE + x]; // Acceso lineal igual de rápido
    }

    const Field& Board::getField(int x, int y) const
    {
        if (!isValid(x, y)) throw std::out_of_range("Out of bounds");
        return m_fields[y * SIZE + x];
    }

    const std::vector<Field>& Board::getAllFields() const
    {
        return m_fields; // Compatible 100%
    }
    
    const std::vector<Wall>& Board::getAllWalls() const
    {
        return m_walls;
    }

    bool Board::placeWall(int x, int y, Orientation orientation)
    {
        if (x < 0 || x >= SIZE - 1 || y < 0 || y >= SIZE - 1) return false;

        for (const auto& w : m_walls) 
        {

            if (w.x() == x && w.y() == y && w.orientation() == orientation) return false;
            if (w.x() == x && w.y() == y) return false;

        }

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
    bool Board::isValid(int x,int y) const{
        return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
    }
}
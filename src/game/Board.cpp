#include "game/Board.hpp"
#include <stdexcept>

namespace Game
{
    Board::Board()
    {
        // El std::array ya está creado en memoria. Solo configuramos las coordenadas.
        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
            {
                int index = y * SIZE + x;
                m_fields[index] = Field(x, y); 
            }
        }
    }

    void Board::init()
    {
        // Reiniciar el tablero sobrescribiendo con Fields nuevos (conexiones en true)
        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
            {
                int index = y * SIZE + x;
                m_fields[index] = Field(x, y);
            }
        }
        m_walls.clear();
    }

    Field& Board::getField(int x, int y)
    {
        if (!isValid(x, y)) {
            throw std::out_of_range("Board::getField - Coordenadas fuera de rango");
        }
        return m_fields[y * SIZE + x];
    }

    const Field& Board::getField(int x, int y) const
    {
        if (!isValid(x, y)) {
            throw std::out_of_range("Board::getField const - Coordenadas fuera de rango");
        }
        return m_fields[y * SIZE + x];
    }

    const std::array<Field, Board::SIZE * Board::SIZE>& Board::getAllFields() const
    {
        return m_fields;
    }

    const std::vector<Wall>& Board::getAllWalls() const
    {
        return m_walls;
    }

    bool Board::placeWall(int x, int y, Orientation orientation)
    {
        // 1. Validar límites del tablero (0..7 para paredes)
        if (x < 0 || x >= SIZE - 1 || y < 0 || y >= SIZE - 1)
            return false;

        // 2. Validar colisión con paredes existentes
        for (const auto& w : m_walls) {
            // Si ya hay una pared exacta
            if (w.x() == x && w.y() == y && w.orientation() == orientation) return false;
            
            // Validación básica de cruce (Horizontal encima de Vertical en el mismo centro)
            if (w.x() == x && w.y() == y) return false;
        }

        // 3. ACTUALIZAR EL GRAFO (Cortar conexiones bidireccionalmente)
        if (orientation == Orientation::Horizontal)
        {
            // Pared Horizontal: Corta verticalmente entre filas y y y+1
            
            // Lado Izquierdo de la pared
            getField(x, y).disconnect(Direction::Down);
            getField(x, y + 1).disconnect(Direction::Up);

            // Lado Derecho de la pared
            getField(x + 1, y).disconnect(Direction::Down);
            getField(x + 1, y + 1).disconnect(Direction::Up);
        }
        else // Vertical
        {
            // Pared Vertical: Corta horizontalmente entre columnas x y x+1

            // Parte Superior de la pared
            getField(x, y).disconnect(Direction::Right);
            getField(x + 1, y).disconnect(Direction::Left);

            // Parte Inferior de la pared
            getField(x, y + 1).disconnect(Direction::Right);
            getField(x + 1, y + 1).disconnect(Direction::Left);
        }

        // 4. Guardar pared física
        m_walls.emplace_back(x, y, orientation);
        return true;
    }
    bool Board::isValid(int x,int y) const{
        return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
    }
}
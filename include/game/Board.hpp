#pragma once
#include "game/Field.hpp"
#include "game/Wall.hpp"
#include <vector>
#include <memory>

namespace Game
{
    class Board
    {
    
        private:
            std::vector<Field> m_fields;
            std::vector<Wall> m_walls;

            // Helper to remove bidirectional connection
            void disconnect(Field *a, Field *b);
            
        public:
            Board();
            static constexpr int SIZE = 9;

            // Initialize grid and neighbor connections
            void init();

            // Graph Operations
            Field *getField(int x, int y);
            const Field *getField(int x, int y) const;
            const std::vector<Field> &getAllFields() const;
            const std::vector<Wall> &getAllWalls() const;

            // Placing a wall cuts connections
            bool placeWall(int x, int y, Orientation orientation);

    };

}
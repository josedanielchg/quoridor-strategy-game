#pragma once
#include "game/Field.hpp"
#include "game/Wall.hpp"
#include <vector>
#include <memory>
#include <array>
#include <optional>

namespace Game
{
    class Board
    {
        public:
            static constexpr int SIZE = 9;
    
        private:
        std::array<Field, SIZE * SIZE> m_fields;

        std::vector<Wall> m_walls;
            
        public:
            Board();
            
            // Initialize grid and neighbor connections
            void init();

            // Graph Operations
            Field& getField(int x, int y);
            const Field& getField(int x, int y) const;

            const std::array<Field, SIZE * SIZE>& getAllFields() const;
            
            const std::vector<Wall> &getAllWalls() const;

            // Placing a wall cuts connections
            bool placeWall(int x, int y, Orientation orientation);
            bool isValid(int x, int y) const;

    };

}

#pragma once
#include "game/Field.hpp"
#include "game/Wall.hpp"
#include "game/Pawn.hpp"
#include <vector>
#include <optional>

namespace Game
{
    class Board
    {
        public:
            static constexpr int SIZE = 9;
    
        private:
        std::vector<Field> m_fields;
        std::vector<Wall> m_walls;
        std::vector<Pawn> m_pawns;
            
        public:
            Board();
            
            // Initialize grid and neighbor connections
            void init();

            // Graph Operations
            Field& getField(int x, int y);
            const Field& getField(int x, int y) const;
            const std::vector<Field>& getAllFields() const;
            
            // Entity Access
            const std::vector<Wall> &getAllWalls() const;
            const std::vector<Pawn>& getAllPawns() const;

            // Helper: Returns pointer to pawn at x,y or nullptr
            const Pawn* getPawnAt(int x, int y) const;
            Pawn* getPawnById(int id);
            
            // Actions
            bool placeWall(int x, int y, Orientation orientation);
            bool movePawn(int pawnId, int targetX, int targetY);

            bool isValid(int x, int y) const;
    };

}

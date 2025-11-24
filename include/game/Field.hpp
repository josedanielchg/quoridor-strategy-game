#pragma once
#include <vector>
#include <algorithm>

namespace Game
{

    class Field
    {
        private:
            int m_x;
            int m_y;
            int m_occupantId;
            std::vector<Field *> m_neighbors;

        public:
            Field(int x, int y);

            // Getters
            int x() const;
            int y() const;
            int occupantId() const; // 0 = None, 1 = P1, 2 = P2

            // Setters
            void setOccupantId(int id);

            // Graph Logic
            void addNeighbor(Field *neighbor);
            void removeNeighbor(Field *neighbor);
            const std::vector<Field *> &getNeighbors() const;

            // Check if connected to specific neighbor
            bool hasNeighbor(const Field *neighbor) const;
    };
}
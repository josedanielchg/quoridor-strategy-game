#include "ai/PathFinder.hpp"
#include "game/Field.hpp"
#include <queue>
#include <cmath>
#include <array>
#include <limits>

namespace Game
{
    // A* search node for a board cell.
    struct Node
    {
        int x, y;
        int gCost; 
        int hCost; 

        // Total priority (F = G + H).
        int fCost() const { return gCost + hCost; }

        // Invert comparison so the priority_queue pops the lowest fCost first.
        bool operator>(const Node& other) const
        {
            return fCost() > other.fCost();
        }
    };

    bool PathFinder::doesPathExist(const Board& board, int startX, int startY, int targetRow)
    {
        return shortestPathLength(board, startX, startY, targetRow) >= 0;
    }

    int PathFinder::shortestPathLength(const Board& board, int startX, int startY, int targetRow)
    {
        // Guard: start must be inside the board.
        if (!board.isValid(startX, startY)) return -1;
        
        if (startY == targetRow) return 0;

        // Open set for A* (nodes to explore next).
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

        // Best known gCost per cell (flat array for speed).
        std::array<int, Board::SIZE * Board::SIZE> minCosts;
        minCosts.fill(std::numeric_limits<int>::max());

        // Seed with the start node.
        int startIdx = startY * Board::SIZE + startX;
        minCosts[startIdx] = 0;

        int hInitial = std::abs(startY - targetRow);
        openSet.push({startX, startY, 0, hInitial});

        while (!openSet.empty())
        {
            Node current = openSet.top();
            openSet.pop();

            if (current.y == targetRow)
            {
                return current.gCost;
            }

            // Skip if we already found a cheaper path to this cell.
            int currentIdx = current.y * Board::SIZE + current.x;
            if (current.gCost > minCosts[currentIdx]) continue;

            // Neighbor offsets: Up, Right, Down, Left.
            const int dx[] = {0, 1, 0, -1};
            const int dy[] = {-1, 0, 1, 0};

            const Field& currentField = board.getField(current.x, current.y);

            for (int i = 0; i < 4; ++i)
            {
                Direction dir = static_cast<Direction>(i);

                // Only move if there is no wall blocking this direction.
                if (currentField.hasPath(dir))
                {
                    int nextX = current.x + dx[i];
                    int nextY = current.y + dy[i];

                    if (board.isValid(nextX, nextY))
                    {
                        // Each step has uniform cost 1.
                        int newGCost = current.gCost + 1;
                        int nextIdx = nextY * Board::SIZE + nextX;

                        if (newGCost < minCosts[nextIdx])
                        {
                            minCosts[nextIdx] = newGCost;
                            
                            // Heuristic: vertical distance to target row.
                            int newH = std::abs(nextY - targetRow);
                            openSet.push({nextX, nextY, newGCost, newH});
                        }
                    }
                }
            }
        }

        return -1;
    }
}

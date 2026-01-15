#include "ai/PathFinder.hpp"
#include "game/Field.hpp"
#include <queue>
#include <cmath>
#include <array>
#include <limits>

namespace Game
{
    struct Node
    {
        int x, y;
        int gCost;
        int hCost;

        int fCost() const { return gCost + hCost; }

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

        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

        std::array<int, Board::SIZE * Board::SIZE> minCosts;
        minCosts.fill(std::numeric_limits<int>::max());

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

            int currentIdx = current.y * Board::SIZE + current.x;
            if (current.gCost > minCosts[currentIdx]) continue;

            const int dx[] = {0, 1, 0, -1};
            const int dy[] = {-1, 0, 1, 0};

            const Field& currentField = board.getField(current.x, current.y);

            for (int i = 0; i < 4; ++i)
            {
                Direction dir = static_cast<Direction>(i);

                if (currentField.hasPath(dir))
                {
                    int nextX = current.x + dx[i];
                    int nextY = current.y + dy[i];

                    if (board.isValid(nextX, nextY))
                    {
                        int newGCost = current.gCost + 1;
                        int nextIdx = nextY * Board::SIZE + nextX;

                        if (newGCost < minCosts[nextIdx])
                        {
                            minCosts[nextIdx] = newGCost;
                            
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

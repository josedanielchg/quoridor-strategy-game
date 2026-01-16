#include "ai/PathFinder.hpp"
#include "game/Field.hpp"
#include <queue>
#include <cmath>
#include <array>
#include <limits>
#include <deque>
#include <chrono>

namespace Game
{
    namespace
    {
        PathFinder::PathStats g_stats;

        static long long msSince(const std::chrono::high_resolution_clock::time_point &start)
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now() - start)
                .count();
        }
    }
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

    static int shortestPathLengthBFS(const Board& board, int startX, int startY, int targetRow)
    {
        auto start = std::chrono::high_resolution_clock::now();
        if (!board.isValid(startX, startY))
        {
            g_stats.astarCalls += 1;
            g_stats.astarMs += msSince(start);
            return -1;
        }
        if (startY == targetRow)
        {
            g_stats.astarCalls += 1;
            g_stats.astarMs += msSince(start);
            return 0;
        }

        std::array<int, Board::SIZE * Board::SIZE> dist;
        dist.fill(-1);
        std::deque<std::pair<int, int>> queue;
        dist[startY * Board::SIZE + startX] = 0;
        queue.push_back({startX, startY});

        const int dx[] = {0, 1, 0, -1};
        const int dy[] = {-1, 0, 1, 0};

        while (!queue.empty())
        {
            auto [x, y] = queue.front();
            queue.pop_front();

            if (y == targetRow)
            {
                g_stats.astarCalls += 1;
                g_stats.astarMs += msSince(start);
                return dist[y * Board::SIZE + x];
            }

            const Field& field = board.getField(x, y);
            for (int i = 0; i < 4; ++i)
            {
                Direction dir = static_cast<Direction>(i);
                if (!field.hasPath(dir))
                    continue;

                int nx = x + dx[i];
                int ny = y + dy[i];
                if (!board.isValid(nx, ny))
                    continue;

                int idx = ny * Board::SIZE + nx;
                if (dist[idx] != -1)
                    continue;

                dist[idx] = dist[y * Board::SIZE + x] + 1;
                queue.push_back({nx, ny});
            }
        }

        g_stats.astarCalls += 1;
        g_stats.astarMs += msSince(start);
        return -1;
    }

    int PathFinder::shortestPathLength(const Board& board, int startX, int startY, int targetRow)
    {
        // BFS is faster than A* for small grids and guarantees shortest path length.
        return shortestPathLengthBFS(board, startX, startY, targetRow);

        auto start = std::chrono::high_resolution_clock::now();
        // Guard: start must be inside the board.
        if (!board.isValid(startX, startY))
        {
            g_stats.astarCalls += 1;
            g_stats.astarMs += msSince(start);
            return -1;
        }
        
        if (startY == targetRow)
        {
            g_stats.astarCalls += 1;
            g_stats.astarMs += msSince(start);
            return 0;
        }

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
                g_stats.astarCalls += 1;
                g_stats.astarMs += msSince(start);
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

        g_stats.astarCalls += 1;
        g_stats.astarMs += msSince(start);
        return -1;
    }

    bool PathFinder::doesPathExistBFS(const Board& board, int startX, int startY, int targetRow)
    {
        auto start = std::chrono::high_resolution_clock::now();
        if (!board.isValid(startX, startY))
        {
            g_stats.bfsCalls += 1;
            g_stats.bfsMs += msSince(start);
            return false;
        }
        if (startY == targetRow)
        {
            g_stats.bfsCalls += 1;
            g_stats.bfsMs += msSince(start);
            return true;
        }

        std::array<bool, Board::SIZE * Board::SIZE> visited;
        visited.fill(false);
        std::deque<std::pair<int, int>> queue;

        visited[startY * Board::SIZE + startX] = true;
        queue.push_back({startX, startY});

        const int dx[] = {0, 1, 0, -1};
        const int dy[] = {-1, 0, 1, 0};

        while (!queue.empty())
        {
            auto [x, y] = queue.front();
            queue.pop_front();

            if (y == targetRow)
            {
                g_stats.bfsCalls += 1;
                g_stats.bfsMs += msSince(start);
                return true;
            }

            const Field& field = board.getField(x, y);

            for (int i = 0; i < 4; ++i)
            {
                Direction dir = static_cast<Direction>(i);
                if (!field.hasPath(dir))
                    continue;

                int nx = x + dx[i];
                int ny = y + dy[i];
                if (!board.isValid(nx, ny))
                    continue;

                int idx = ny * Board::SIZE + nx;
                if (visited[idx])
                    continue;

                visited[idx] = true;
                queue.push_back({nx, ny});
            }
        }

        g_stats.bfsCalls += 1;
        g_stats.bfsMs += msSince(start);
        return false;
    }

    void PathFinder::resetStats()
    {
        g_stats = PathStats{};
    }

    PathFinder::PathStats PathFinder::getStats()
    {
        return g_stats;
    }
}

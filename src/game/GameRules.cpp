#include "game/GameRules.hpp"
#include "game/WallRules.hpp"

#include <cmath>
#include <cstring>
#include <queue>
#include <utility>

namespace Game
{
    static_assert(GameState::WALL_GRID == WALL_GRID, "Wall grid size mismatch.");

    void initGameState(GameState &state)
    {
        state.pawnX[0] = 4;
        state.pawnY[0] = GameState::BOARD_SIZE - 1;
        state.pawnX[1] = 4;
        state.pawnY[1] = 0;

        state.wallsRemaining[0] = GameState::MAX_WALLS_PER_PLAYER;
        state.wallsRemaining[1] = GameState::MAX_WALLS_PER_PLAYER;

        std::memset(state.hWalls, 0, sizeof(state.hWalls));
        std::memset(state.vWalls, 0, sizeof(state.vWalls));
        std::memset(state.distToGoal, 0xFF, sizeof(state.distToGoal));
        state.distDirty = 1;

        state.currentPlayerId = 1;
        state.winnerId = 0;
    }

    namespace
    {
        bool inBoundsCell(int x, int y)
        {
            return x >= 0 && x < GameState::BOARD_SIZE && y >= 0 && y < GameState::BOARD_SIZE;
        }

        bool inBoundsWall(int x, int y)
        {
            return x >= 0 && x < GameState::WALL_GRID && y >= 0 && y < GameState::WALL_GRID;
        }

        bool hasHorizontalWall(const GameState &state, int x, int y)
        {
            return inBoundsWall(x, y) && state.hWalls[x][y] != 0;
        }

        bool hasVerticalWall(const GameState &state, int x, int y)
        {
            return inBoundsWall(x, y) && state.vWalls[x][y] != 0;
        }

        bool isOccupied(const GameState &state, int x, int y)
        {
            return (state.pawnX[0] == x && state.pawnY[0] == y) ||
                   (state.pawnX[1] == x && state.pawnY[1] == y);
        }

        bool isOpponentAt(const GameState &state, int playerId, int x, int y)
        {
            int opponentIdx = (playerId == 1) ? 1 : 0;
            return state.pawnX[opponentIdx] == x && state.pawnY[opponentIdx] == y;
        }

        bool isBlockedBetween(const GameState &state, int x, int y, int nx, int ny)
        {
            int dx = nx - x;
            int dy = ny - y;

            if (dx == 0 && dy == 1)
            {
                return hasHorizontalWall(state, x, y) || hasHorizontalWall(state, x - 1, y);
            }
            if (dx == 0 && dy == -1)
            {
                return hasHorizontalWall(state, x, y - 1) || hasHorizontalWall(state, x - 1, y - 1);
            }
            if (dx == 1 && dy == 0)
            {
                return hasVerticalWall(state, x, y) || hasVerticalWall(state, x, y - 1);
            }
            if (dx == -1 && dy == 0)
            {
                return hasVerticalWall(state, x - 1, y) || hasVerticalWall(state, x - 1, y - 1);
            }

            return true;
        }
    }

    int currentPlayer(const GameState &state) { return state.currentPlayerId; }
    int winner(const GameState &state) { return state.winnerId; }
    bool isGameOver(const GameState &state) { return state.winnerId != 0; }

    void computeDistancesToGoal(const GameState &state, int playerId,
                                int16_t dist[GameState::BOARD_SIZE][GameState::BOARD_SIZE])
    {
        for (int y = 0; y < GameState::BOARD_SIZE; ++y)
        {
            for (int x = 0; x < GameState::BOARD_SIZE; ++x)
            {
                dist[y][x] = -1;
            }
        }

        if (playerId != 1 && playerId != 2)
            return;

        int targetRow = (playerId == 1) ? 0 : GameState::BOARD_SIZE - 1;
        std::queue<std::pair<int, int>> open;

        for (int x = 0; x < GameState::BOARD_SIZE; ++x)
        {
            dist[targetRow][x] = 0;
            open.push({x, targetRow});
        }

        const int dx[] = {0, 1, 0, -1};
        const int dy[] = {-1, 0, 1, 0};

        while (!open.empty())
        {
            auto current = open.front();
            open.pop();

            int currentDist = dist[current.second][current.first];

            for (int i = 0; i < 4; ++i)
            {
                int nx = current.first + dx[i];
                int ny = current.second + dy[i];
                if (!inBoundsCell(nx, ny))
                    continue;
                if (dist[ny][nx] != -1)
                    continue;
                if (isBlockedBetween(state, current.first, current.second, nx, ny))
                    continue;

                dist[ny][nx] = static_cast<int16_t>(currentDist + 1);
                open.push({nx, ny});
            }
        }
    }

    void updateDistanceCache(GameState &state)
    {
        if (state.distDirty == 0)
            return;

        computeDistancesToGoal(state, 1, state.distToGoal[0]);
        computeDistancesToGoal(state, 2, state.distToGoal[1]);
        state.distDirty = 0;
    }

    bool isPawnMoveValid(const GameState &state, int playerId, int targetX, int targetY)
    {
        if (playerId != 1 && playerId != 2)
            return false;
        if (!inBoundsCell(targetX, targetY))
            return false;
        if (isOccupied(state, targetX, targetY))
            return false;

        int idx = playerId - 1;
        int startX = state.pawnX[idx];
        int startY = state.pawnY[idx];
        int dx = targetX - startX;
        int dy = targetY - startY;
        int distance = std::abs(dx) + std::abs(dy);

        if (distance != 1 && distance != 2)
            return false;

        int dirX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
        int dirY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

        if (dirX == 1 && isBlockedBetween(state, startX, startY, startX + 1, startY))
            return false;
        if (dirX == -1 && isBlockedBetween(state, startX, startY, startX - 1, startY))
            return false;
        if (dirY == 1 && isBlockedBetween(state, startX, startY, startX, startY + 1))
            return false;
        if (dirY == -1 && isBlockedBetween(state, startX, startY, startX, startY - 1))
            return false;

        if (distance == 1)
            return true;

        int intermediateX = startX + dirX;
        int intermediateY = startY + dirY;
        if (!inBoundsCell(intermediateX, intermediateY))
            return false;
        if (!isOpponentAt(state, playerId, intermediateX, intermediateY))
            return false;

        if (dirX == 1 && isBlockedBetween(state, intermediateX, intermediateY, intermediateX + 1, intermediateY))
            return false;
        if (dirX == -1 && isBlockedBetween(state, intermediateX, intermediateY, intermediateX - 1, intermediateY))
            return false;
        if (dirY == 1 && isBlockedBetween(state, intermediateX, intermediateY, intermediateX, intermediateY + 1))
            return false;
        if (dirY == -1 && isBlockedBetween(state, intermediateX, intermediateY, intermediateX, intermediateY - 1))
            return false;

        return true;
    }

    bool isWallPlacementValid(const GameState &state, int playerId, int x, int y, Orientation orientation)
    {
        if (playerId != 1 && playerId != 2)
            return false;

        int idx = playerId - 1;
        if (state.wallsRemaining[idx] == 0)
            return false;

        if (!isWallPlacementLegalLocal(state.hWalls, state.vWalls, x, y, orientation))
            return false;

        GameState temp = state;
        if (orientation == Orientation::Horizontal)
            temp.hWalls[x][y] = 1;
        else
            temp.vWalls[x][y] = 1;

        temp.distDirty = 1;
        updateDistanceCache(temp);

        int16_t p1Dist = temp.distToGoal[0][temp.pawnY[0]][temp.pawnX[0]];
        int16_t p2Dist = temp.distToGoal[1][temp.pawnY[1]][temp.pawnX[1]];
        if (p1Dist < 0 || p2Dist < 0)
            return false;

        return true;
    }

    bool hasPlayerWon(const GameState &state, int playerId)
    {
        if (playerId == 1)
            return state.pawnY[0] == 0;
        if (playerId == 2)
            return state.pawnY[1] == GameState::BOARD_SIZE - 1;
        return false;
    }

    bool applyMoveUnchecked(GameState &state, const Move &move, MoveUndoState &undoState)
    {
        int playerId = move.playerId();
        if (playerId == 0)
            playerId = state.currentPlayerId;
        if (playerId != 1 && playerId != 2)
            return false;

        int idx = playerId - 1;
        undoState.pawnX = state.pawnX[idx];
        undoState.pawnY = state.pawnY[idx];
        undoState.wallsRemaining = state.wallsRemaining[idx];
        undoState.currentPlayerId = state.currentPlayerId;
        undoState.winnerId = state.winnerId;
        undoState.distDirty = state.distDirty;

        if (move.type() == MoveType::PawnMove)
        {
            int x = move.x();
            int y = move.y();
            if (!inBoundsCell(x, y))
                return false;
            state.pawnX[idx] = static_cast<uint8_t>(x);
            state.pawnY[idx] = static_cast<uint8_t>(y);
        }
        else if (move.type() == MoveType::WallPlacement)
        {
            int x = move.x();
            int y = move.y();
            if (!inBoundsWall(x, y))
                return false;

            if (move.orientation() == Orientation::Horizontal)
                state.hWalls[x][y] = 1;
            else
                state.vWalls[x][y] = 1;

            state.wallsRemaining[idx] =
                static_cast<uint8_t>(state.wallsRemaining[idx] - 1);
            state.distDirty = 1;
        }
        else
        {
            return false;
        }

        if (hasPlayerWon(state, playerId))
            state.winnerId = static_cast<uint8_t>(playerId);
        state.currentPlayerId = (playerId == 1) ? 2 : 1;

        return true;
    }

    void undoMove(GameState &state, const Move &move, const MoveUndoState &undoState)
    {
        int playerId = undoState.currentPlayerId;
        if (playerId != 1 && playerId != 2)
            return;

        int idx = playerId - 1;
        state.pawnX[idx] = undoState.pawnX;
        state.pawnY[idx] = undoState.pawnY;
        state.wallsRemaining[idx] = undoState.wallsRemaining;
        state.currentPlayerId = undoState.currentPlayerId;
        state.winnerId = undoState.winnerId;
        state.distDirty = undoState.distDirty;

        if (move.type() == MoveType::WallPlacement)
        {
            int x = move.x();
            int y = move.y();
            if (inBoundsWall(x, y))
            {
                if (move.orientation() == Orientation::Horizontal)
                    state.hWalls[x][y] = 0;
                else
                    state.vWalls[x][y] = 0;
            }
            state.distDirty = 1;
        }
    }

    bool applyMove(GameState &state, const Move &move)
    {
        int playerId = move.playerId();
        if (playerId == 0)
        {
            playerId = state.currentPlayerId;
        }
        else if (playerId != state.currentPlayerId)
        {
            return false;
        }

        if (move.type() == MoveType::PawnMove)
        {
            if (!isPawnMoveValid(state, playerId, move.x(), move.y()))
                return false;
        }
        else if (move.type() == MoveType::WallPlacement)
        {
            if (!isWallPlacementValid(state, playerId, move.x(), move.y(), move.orientation()))
                return false;
        }
        else
        {
            return false;
        }

        MoveUndoState undoState{};
        return applyMoveUnchecked(state, move, undoState);
    }

    std::vector<Move> generateLegalMoves(const GameState &state)
    {
        std::vector<Move> moves;
        int playerId = state.currentPlayerId;

        for (int y = 0; y < GameState::BOARD_SIZE; ++y)
        {
            for (int x = 0; x < GameState::BOARD_SIZE; ++x)
            {
                if (isPawnMoveValid(state, playerId, x, y))
                    moves.push_back(Move::Pawn(x, y, playerId));
            }
        }

        if (state.wallsRemaining[playerId - 1] > 0)
        {
            for (int y = 0; y < GameState::WALL_GRID; ++y)
            {
                for (int x = 0; x < GameState::WALL_GRID; ++x)
                {
                    if (isWallPlacementLegalLocal(state.hWalls, state.vWalls, x, y, Orientation::Horizontal))
                        moves.push_back(Move::Wall(x, y, Orientation::Horizontal, playerId));
                    if (isWallPlacementLegalLocal(state.hWalls, state.vWalls, x, y, Orientation::Vertical))
                        moves.push_back(Move::Wall(x, y, Orientation::Vertical, playerId));
                }
            }
        }

        return moves;
    }

    int evaluateState(GameState &state, int perspectivePlayerId)
    {
        updateDistanceCache(state);

        if (perspectivePlayerId != 1 && perspectivePlayerId != 2)
            return 0;

        int myIdx = perspectivePlayerId - 1;
        int oppIdx = (myIdx == 0) ? 1 : 0;

        int16_t myDist = state.distToGoal[myIdx][state.pawnY[myIdx]][state.pawnX[myIdx]];
        int16_t oppDist = state.distToGoal[oppIdx][state.pawnY[oppIdx]][state.pawnX[oppIdx]];

        if (myDist < 0)
            return -100000;
        if (oppDist < 0)
            return 100000;

        int distScore = static_cast<int>(oppDist - myDist);
        int wallScore = static_cast<int>(state.wallsRemaining[myIdx]) -
                        static_cast<int>(state.wallsRemaining[oppIdx]);

        return distScore * 10 + wallScore * 2;
    }
}

#include "heuristic/HeuristicEngine.hpp"
#include "game/GameRules.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>

namespace Game
{
    namespace
    {
        constexpr int kWinScore = 1000000;

        struct SearchContext
        {
            const HeuristicSearchConfig *config = nullptr;
            TranspositionTable *tt = nullptr;
            std::chrono::steady_clock::time_point deadline;
            int rootPlayerId = 1;
            int64_t nodes = 0;
            bool timeUp = false;
        };

        constexpr int kTTMoveBonus = 10000000;
        constexpr int kPawnReduceDistBonus = 1000;
        constexpr int kWallBlocksOppBonus = 1000;
        constexpr int kWallBlocksSelfPenalty = 700;
        constexpr int kWallCloseToOppBonus = 200;
        constexpr int kWallCloseRadiusOpp = 2;
        constexpr int kWallCloseRadiusSelf = 1;
        constexpr size_t kMaxWallMoves = 32;

        bool sameMove(const Move &a, const Move &b)
        {
            if (a.type() != b.type())
                return false;
            if (a.x() != b.x() || a.y() != b.y())
                return false;
            if (a.type() == MoveType::WallPlacement && a.orientation() != b.orientation())
                return false;
            if (a.playerId() != 0 && b.playerId() != 0 && a.playerId() != b.playerId())
                return false;
            return true;
        }

        int manhattan(int ax, int ay, int bx, int by)
        {
            return std::abs(ax - bx) + std::abs(ay - by);
        }

        int wallMinDistanceToPawn(int wallX, int wallY, int pawnX, int pawnY)
        {
            int best = std::numeric_limits<int>::max();
            best = std::min(best, manhattan(wallX, wallY, pawnX, pawnY));
            best = std::min(best, manhattan(wallX + 1, wallY, pawnX, pawnY));
            best = std::min(best, manhattan(wallX, wallY + 1, pawnX, pawnY));
            best = std::min(best, manhattan(wallX + 1, wallY + 1, pawnX, pawnY));
            return best;
        }

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

        bool isReducingEdgeForPlayer(const GameState &state, int playerIdx,
                                     int ax, int ay, int bx, int by)
        {
            int16_t distA = state.distToGoal[playerIdx][ay][ax];
            int16_t distB = state.distToGoal[playerIdx][by][bx];
            if (distA < 0 || distB < 0)
                return false;
            return (distA == distB + 1) || (distB == distA + 1);
        }

        int countBlockedReducingEdgesByWall(const GameState &state, int playerIdx,
                                            int wallX, int wallY, Orientation orientation)
        {
            if (!inBoundsWall(wallX, wallY))
                return 0;

            int blocked = 0;

            auto considerEdge = [&](int ax, int ay, int bx, int by)
            {
                if (!inBoundsCell(ax, ay) || !inBoundsCell(bx, by))
                    return;
                if (isBlockedBetween(state, ax, ay, bx, by))
                    return;
                if (isReducingEdgeForPlayer(state, playerIdx, ax, ay, bx, by))
                    blocked++;
            };

            if (orientation == Orientation::Horizontal)
            {
                considerEdge(wallX, wallY, wallX, wallY + 1);
                considerEdge(wallX + 1, wallY, wallX + 1, wallY + 1);
            }
            else
            {
                considerEdge(wallX, wallY, wallX + 1, wallY);
                considerEdge(wallX, wallY + 1, wallX + 1, wallY + 1);
            }

            return blocked;
        }

        int countReducingNeighbors(const GameState &state, int playerIdx)
        {
            int x = state.pawnX[playerIdx];
            int y = state.pawnY[playerIdx];
            if (!inBoundsCell(x, y))
                return 0;

            int16_t dist = state.distToGoal[playerIdx][y][x];
            if (dist <= 0)
                return 0;

            int count = 0;
            const int dx[] = {0, 1, 0, -1};
            const int dy[] = {-1, 0, 1, 0};

            for (int i = 0; i < 4; ++i)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (!inBoundsCell(nx, ny))
                    continue;
                if (isBlockedBetween(state, x, y, nx, ny))
                    continue;
                if (state.distToGoal[playerIdx][ny][nx] == dist - 1)
                    count++;
            }

            return count;
        }

        int evaluateHeuristic(GameState &state, const HeuristicSearchConfig &config, int perspectivePlayerId)
        {
            updateDistanceCache(state);

            if (perspectivePlayerId != 1 && perspectivePlayerId != 2)
                return 0;

            int myIdx = perspectivePlayerId - 1;
            int oppIdx = (myIdx == 0) ? 1 : 0;

            int16_t myDist = state.distToGoal[myIdx][state.pawnY[myIdx]][state.pawnX[myIdx]];
            int16_t oppDist = state.distToGoal[oppIdx][state.pawnY[oppIdx]][state.pawnX[oppIdx]];

            if (myDist < 0)
                return -kWinScore / 2;
            if (oppDist < 0)
                return kWinScore / 2;

            int distScore = static_cast<int>(oppDist - myDist);
            int wallScore = static_cast<int>(state.wallsRemaining[myIdx]) -
                            static_cast<int>(state.wallsRemaining[oppIdx]);
            int widthScore = countReducingNeighbors(state, myIdx) -
                             countReducingNeighbors(state, oppIdx);

            return distScore * config.weightDistance +
                   wallScore * config.weightWalls +
                   widthScore * config.weightWidth;
        }

        std::vector<Move> generateOrderedMoves(GameState &state, SearchContext &ctx,
                                               const std::optional<Move> &ttBestMove)
        {
            updateDistanceCache(state);

            const int currentPlayerId = state.currentPlayerId;
            const int currentIdx = (currentPlayerId == 2) ? 1 : 0;
            const int opponentIdx = (currentIdx == 0) ? 1 : 0;

            const int pawnXSelf = state.pawnX[currentIdx];
            const int pawnYSelf = state.pawnY[currentIdx];
            const int pawnXOpp = state.pawnX[opponentIdx];
            const int pawnYOpp = state.pawnY[opponentIdx];

            const int16_t currentDist =
                state.distToGoal[currentIdx][pawnYSelf][pawnXSelf];

            struct ScoredMove
            {
                Move move;
                int score;
                bool wall;
                bool wallPassesFilter;
            };

            std::vector<ScoredMove> scoredMoves;
            std::vector<ScoredMove> wallMoves;

            std::vector<Move> moves = generateLegalMoves(state);
            const bool noWallsRemaining = (state.wallsRemaining[currentIdx] == 0);
            bool hasReducingPawnMove = false;
            if (noWallsRemaining && currentDist >= 0)
            {
                for (const Move &move : moves)
                {
                    if (move.type() != MoveType::PawnMove)
                        continue;

                    int tx = move.x();
                    int ty = move.y();
                    if (!inBoundsCell(tx, ty))
                        continue;

                    int16_t targetDist = state.distToGoal[currentIdx][ty][tx];
                    if (targetDist >= 0 && targetDist < currentDist)
                    {
                        hasReducingPawnMove = true;
                        break;
                    }
                }
            }
            scoredMoves.reserve(moves.size());

            for (const Move &move : moves)
            {
                int score = 0;
                if (ttBestMove && sameMove(move, *ttBestMove))
                    score += kTTMoveBonus;

                if (move.type() == MoveType::PawnMove)
                {
                    int tx = move.x();
                    int ty = move.y();
                    int16_t targetDist = -1;
                    if (inBoundsCell(tx, ty))
                        targetDist = state.distToGoal[currentIdx][ty][tx];

                    if (noWallsRemaining && hasReducingPawnMove)
                    {
                        if (!(currentDist >= 0 && targetDist >= 0 && targetDist < currentDist))
                            continue;
                    }

                    if (currentDist >= 0 && targetDist >= 0)
                    {
                        int improvement = static_cast<int>(currentDist - targetDist);
                        score += improvement * kPawnReduceDistBonus;
                    }
                }
                else if (move.type() == MoveType::WallPlacement)
                {
                    int wx = move.x();
                    int wy = move.y();
                    Orientation ori = move.orientation();

                    int blocksOpp = countBlockedReducingEdgesByWall(state, opponentIdx, wx, wy, ori);
                    int blocksSelf = countBlockedReducingEdgesByWall(state, currentIdx, wx, wy, ori);

                    score += blocksOpp * kWallBlocksOppBonus;
                    score -= blocksSelf * kWallBlocksSelfPenalty;

                    int distToOpp = wallMinDistanceToPawn(wx, wy, pawnXOpp, pawnYOpp);
                    int distToSelf = wallMinDistanceToPawn(wx, wy, pawnXSelf, pawnYSelf);

                    if (distToOpp <= kWallCloseRadiusOpp)
                        score += (kWallCloseRadiusOpp + 1 - distToOpp) * kWallCloseToOppBonus;

                    bool passesFilter =
                        (blocksOpp > 0) || (distToOpp <= kWallCloseRadiusOpp) || (distToSelf <= kWallCloseRadiusSelf);

                    wallMoves.push_back({move, score, true, passesFilter});
                    continue;
                }

                scoredMoves.push_back({move, score, false, true});
            }

            std::vector<ScoredMove> filteredWalls;
            filteredWalls.reserve(wallMoves.size());
            for (const auto &wall : wallMoves)
            {
                if (wall.wallPassesFilter)
                    filteredWalls.push_back(wall);
            }
            if (filteredWalls.empty())
                filteredWalls = wallMoves;

            std::stable_sort(filteredWalls.begin(), filteredWalls.end(),
                             [](const ScoredMove &a, const ScoredMove &b)
                             { return a.score > b.score; });
            if (filteredWalls.size() > kMaxWallMoves)
                filteredWalls.erase(filteredWalls.begin() + static_cast<std::ptrdiff_t>(kMaxWallMoves),
                                    filteredWalls.end());

            scoredMoves.insert(scoredMoves.end(), filteredWalls.begin(), filteredWalls.end());

            std::stable_sort(scoredMoves.begin(), scoredMoves.end(),
                             [](const ScoredMove &a, const ScoredMove &b)
                             { return a.score > b.score; });

            std::vector<Move> ordered;
            ordered.reserve(scoredMoves.size());
            for (const auto &entry : scoredMoves)
                ordered.push_back(entry.move);
            return ordered;
        }

        bool isMoveValidForState(const GameState &state, const Move &move)
        {
            int playerId = move.playerId();
            if (playerId == 0)
                playerId = state.currentPlayerId;

            if (move.type() == MoveType::PawnMove)
                return isPawnMoveValid(state, playerId, move.x(), move.y());
            if (move.type() == MoveType::WallPlacement)
                return isWallPlacementValid(state, playerId, move.x(), move.y(), move.orientation());

            return false;
        }

        int evaluateTerminal(GameState &state, const HeuristicSearchConfig &config,
                             int rootPlayerId, int depth)
        {
            if (state.winnerId == 0)
                return evaluateHeuristic(state, config, rootPlayerId);

            if (state.winnerId == rootPlayerId)
                return kWinScore + depth;
            return -kWinScore - depth;
        }

        int alphaBeta(GameState &state, int depth, int alpha, int beta, SearchContext &ctx)
        {
            if (ctx.timeUp)
                return evaluateHeuristic(state, *ctx.config, ctx.rootPlayerId);

            if (std::chrono::steady_clock::now() >= ctx.deadline)
            {
                ctx.timeUp = true;
                return evaluateHeuristic(state, *ctx.config, ctx.rootPlayerId);
            }

            ctx.nodes++;

            if (depth <= 0 || state.winnerId != 0)
                return evaluateTerminal(state, *ctx.config, ctx.rootPlayerId, depth);

            const uint64_t key = computeZobrist(state);
            std::optional<Move> ttMove;
            const TTEntry *entry = nullptr;
            if (ctx.tt)
            {
                entry = ctx.tt->probe(key);
                if (entry && entry->bestMove)
                    ttMove = *entry->bestMove;

                if (entry)
                {
                    if (entry->depth >= depth)
                    {
                        if (entry->flag == TTFlag::Exact)
                            return entry->value;
                        if (entry->flag == TTFlag::LowerBound)
                            alpha = std::max(alpha, entry->value);
                        else if (entry->flag == TTFlag::UpperBound)
                            beta = std::min(beta, entry->value);
                        if (alpha >= beta)
                            return entry->value;
                    }
                }
            }

            std::vector<Move> moves = generateOrderedMoves(state, ctx, ttMove);
            if (moves.empty())
                return evaluateTerminal(state, *ctx.config, ctx.rootPlayerId, depth);

            const int alphaOrig = alpha;
            const int betaOrig = beta;

            const bool maximizing = (state.currentPlayerId == ctx.rootPlayerId);
            int bestValue = maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
            Move bestMove = moves.front();

            for (const Move &move : moves)
            {
                if (ctx.timeUp)
                    break;

                if (!isMoveValidForState(state, move))
                    continue;

                MoveUndoState undo{};
                if (!applyMoveUnchecked(state, move, undo))
                    continue;

                int value = alphaBeta(state, depth - 1, alpha, beta, ctx);
                undoMove(state, move, undo);

                if (maximizing)
                {
                    if (value > bestValue)
                    {
                        bestValue = value;
                        bestMove = move;
                    }
                    alpha = std::max(alpha, bestValue);
                }
                else
                {
                    if (value < bestValue)
                    {
                        bestValue = value;
                        bestMove = move;
                    }
                    beta = std::min(beta, bestValue);
                }

                if (beta <= alpha)
                    break;
            }

            if (!ctx.timeUp && ctx.tt)
            {
                TTFlag flag = TTFlag::Exact;
                if (bestValue <= alphaOrig)
                    flag = TTFlag::UpperBound;
                else if (bestValue >= betaOrig)
                    flag = TTFlag::LowerBound;

                ctx.tt->store(key, depth, bestValue, flag, bestMove);
            }

            return bestValue;
        }
    }

    HeuristicEngine::HeuristicEngine(HeuristicSearchConfig config)
        : m_config(config), m_tt(1 << 20)
    {
    }

    void HeuristicEngine::setConfig(const HeuristicSearchConfig &config)
    {
        m_config = config;
    }

    const HeuristicSearchConfig &HeuristicEngine::config() const
    {
        return m_config;
    }

    Move HeuristicEngine::findBestMove(const GameState &state)
    {
        GameState working = state;

        m_tt.clear();

        SearchContext ctx{};
        ctx.config = &m_config;
        ctx.tt = &m_tt;
        ctx.rootPlayerId = state.currentPlayerId;
        ctx.deadline = std::chrono::steady_clock::now() +
                       std::chrono::milliseconds(m_config.timeLimitMs);

        std::vector<Move> moves = generateOrderedMoves(working, ctx, std::nullopt);
        if (moves.empty())
        {
            int playerId = state.currentPlayerId;
            int idx = (playerId == 2) ? 1 : 0;
            return Move::Pawn(state.pawnX[idx], state.pawnY[idx], playerId);
        }

        int bestScore = std::numeric_limits<int>::min();
        Move bestMove = moves.front();

        for (const Move &move : moves)
        {
            if (ctx.timeUp)
                break;

            if (!isMoveValidForState(working, move))
                continue;

            MoveUndoState undo{};
            if (!applyMoveUnchecked(working, move, undo))
                continue;

            int value = alphaBeta(working, m_config.maxDepth - 1,
                                  std::numeric_limits<int>::min(),
                                  std::numeric_limits<int>::max(), ctx);
            undoMove(working, move, undo);

            if (value > bestScore)
            {
                bestScore = value;
                bestMove = move;
            }
        }

        return bestMove;
    }
}

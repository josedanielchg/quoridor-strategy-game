#include "ai/AiEngine.hpp"
#include "ai/PathFinder.hpp"
#include "game/Move.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>

namespace Game
{
    namespace
    {
        struct ProfileStats
        {
            long long selectCalls = 0;
            long long selectMs = 0;
            long long evalCalls = 0;
            long long evalMs = 0;
            long long negamaxCalls = 0;
            long long negamaxMs = 0;
            long long winCheckCalls = 0;
            long long winCheckMs = 0;
            long long applyMoveCalls = 0;
            long long applyMoveMs = 0;
            long long applyUncheckedCalls = 0;
            long long applyUncheckedMs = 0;

            void reset()
            {
                selectCalls = 0;
                selectMs = 0;
                evalCalls = 0;
                evalMs = 0;
                negamaxCalls = 0;
                negamaxMs = 0;
                winCheckCalls = 0;
                winCheckMs = 0;
                applyMoveCalls = 0;
                applyMoveMs = 0;
                applyUncheckedCalls = 0;
                applyUncheckedMs = 0;
            }
        };

        static ProfileStats g_profile;

        static long long msSince(const std::chrono::high_resolution_clock::time_point &start)
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now() - start)
                .count();
        }

        std::vector<Move> selectMoves(const GameState &state, int maxWalls)
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<Move> moves = state.generateLegalMoves();
            std::vector<Move> pawnMoves;
            std::vector<Move> wallMoves;
            pawnMoves.reserve(moves.size());
            wallMoves.reserve(moves.size());

            for (const auto &move : moves)
            {
                if (move.type() == MoveType::PawnMove)
                    pawnMoves.push_back(move);
                else
                    wallMoves.push_back(move);
            }

            if (maxWalls >= 0 && wallMoves.size() > static_cast<size_t>(maxWalls))
            {
                int opponentId = (state.currentPlayer() == 1) ? 2 : 1;
                PawnState opp = state.pawn(opponentId);
                std::sort(wallMoves.begin(), wallMoves.end(),
                          [&](const Move &a, const Move &b)
                          {
                              int da = std::abs(a.x() - opp.x) + std::abs(a.y() - opp.y);
                              int db = std::abs(b.x() - opp.x) + std::abs(b.y() - opp.y);
                              return da < db;
                          });
                wallMoves.erase(wallMoves.begin() + maxWalls, wallMoves.end());
            }

            pawnMoves.insert(pawnMoves.end(), wallMoves.begin(), wallMoves.end());
            g_profile.selectCalls += 1;
            g_profile.selectMs += msSince(start);
            return pawnMoves;
        }
    }

    int AiEngine::evaluate(const GameState &state, int maximizingPlayerId)
    {
        auto start = std::chrono::high_resolution_clock::now();
        Board board;
        if (!state.syncBoard(board))
            return std::numeric_limits<int>::min();

        const PawnState p1 = state.pawn(1);
        const PawnState p2 = state.pawn(2);

        int distP1 = PathFinder::shortestPathLength(board, p1.x, p1.y, 0);
        int distP2 = PathFinder::shortestPathLength(board, p2.x, p2.y, Board::SIZE - 1);

        if (distP1 < 0)
            distP1 = 1000;
        if (distP2 < 0)
            distP2 = 1000;

        // Strong bonus for a winning position.
        if (distP1 == 0)
            return (maximizingPlayerId == 1) ? 100000 : -100000;
        if (distP2 == 0)
            return (maximizingPlayerId == 2) ? 100000 : -100000;

        int wallDiff = state.wallsRemaining(1) - state.wallsRemaining(2);
        // Positive score means good for player 1 by default.
        int score = (distP2 - distP1) * 10 + wallDiff;

        // Flip the perspective if player 2 is the maximizer.
        if (maximizingPlayerId == 2)
            score = -score;

        g_profile.evalCalls += 1;
        g_profile.evalMs += msSince(start);
        return score;
    }

    Move AiEngine::findBestMove(const GameState &state, int depth)
    {
        g_profile.reset();
        auto start = std::chrono::high_resolution_clock::now();
        PathFinder::resetStats();
        std::vector<Move> moves = selectMoves(state, 6);
        if (moves.empty())
            // Fallback: return a no-op style move to avoid invalid output.
            return Move::Pawn(state.pawn(state.currentPlayer()).x,
                              state.pawn(state.currentPlayer()).y,
                              state.currentPlayer());

        // Immediate win check: if a move wins now, take it (fast path).
        for (const auto &move : moves)
        {
            auto winStart = std::chrono::high_resolution_clock::now();
            GameState nextState = state;
            auto applyStart = std::chrono::high_resolution_clock::now();
            bool applied = nextState.applyMoveUnchecked(move);
            g_profile.applyUncheckedCalls += 1;
            g_profile.applyUncheckedMs += msSince(applyStart);
            if (!applied)
            {
                g_profile.winCheckCalls += 1;
                g_profile.winCheckMs += msSince(winStart);
                continue;
            }
            if (nextState.hasPlayerWon(state.currentPlayer()))
            {
                g_profile.winCheckCalls += 1;
                g_profile.winCheckMs += msSince(winStart);
                return move;
            }
            g_profile.winCheckCalls += 1;
            g_profile.winCheckMs += msSince(winStart);
        }

        int bestScore = std::numeric_limits<int>::min();
        Move bestMove = moves.front();
        int playerId = state.currentPlayer();
        int targetRow = (playerId == 1) ? 0 : (Board::SIZE - 1);
        int currentY = state.pawn(playerId).y;
        int bestProgress = std::numeric_limits<int>::min();

        for (const auto &move : moves)
        {
            // Simulate this move on a copy of the state.
            GameState nextState = state;
            auto applyStart = std::chrono::high_resolution_clock::now();
            bool applied = nextState.applyMoveUnchecked(move);
            g_profile.applyUncheckedCalls += 1;
            g_profile.applyUncheckedMs += msSince(applyStart);
            if (!applied)
                continue;

            // NegaMax returns score from the current player's perspective.
            int score = -negamax(nextState, depth - 1,
                                 std::numeric_limits<int>::min(),
                                 std::numeric_limits<int>::max(),
                                 playerId);

            int progress = std::abs(currentY - targetRow) - std::abs(move.y() - targetRow);
            if (score > bestScore || (score == bestScore && progress > bestProgress))
            {
                bestScore = score;
                bestMove = move;
                bestProgress = progress;
            }
        }

        PathFinder::PathStats pf = PathFinder::getStats();
        std::cout << "[AI] findBestMove " << msSince(start) << " ms | "
                  << "select=" << g_profile.selectMs << "ms (" << g_profile.selectCalls << " calls), "
                  << "negamax=" << g_profile.negamaxMs << "ms (" << g_profile.negamaxCalls << " calls), "
                  << "eval=" << g_profile.evalMs << "ms (" << g_profile.evalCalls << " calls), "
                  << "winCheck=" << g_profile.winCheckMs << "ms (" << g_profile.winCheckCalls << " calls), "
                  << "applyMove=" << g_profile.applyMoveMs << "ms (" << g_profile.applyMoveCalls << " calls), "
                  << "applyUnchecked=" << g_profile.applyUncheckedMs << "ms (" << g_profile.applyUncheckedCalls << " calls), "
                  << "bfs=" << pf.bfsMs << "ms (" << pf.bfsCalls << " calls), "
                  << "astar=" << pf.astarMs << "ms (" << pf.astarCalls << " calls)\n";

        return bestMove;
    }

    int AiEngine::negamax(GameState state, int depth, int alpha, int beta, int maximizingPlayerId)
    {
        auto start = std::chrono::high_resolution_clock::now();
        // Base case: evaluate the leaf or finished game.
        if (depth <= 0 || state.isGameOver())
            return evaluate(state, state.currentPlayer());

        int maxWalls = (depth <= 1) ? 6 : 3;
        std::vector<Move> moves = selectMoves(state, maxWalls);
        if (moves.empty())
            return evaluate(state, state.currentPlayer());

        int best = std::numeric_limits<int>::min();

        for (const auto &move : moves)
        {
            // Recursively search the opponent's response.
            GameState nextState = state;
            if (!nextState.applyMoveUnchecked(move))
                continue;

            // Flip alpha/beta because perspective changes each ply.
            int score = -negamax(nextState, depth - 1, -beta, -alpha, maximizingPlayerId);

            if (score > best)
                best = score;

            // Alpha-beta pruning: keep the best lower bound for this node.
            if (score > alpha)
                alpha = score;

            // Cut off branches that cannot improve the result.
            if (alpha >= beta)
                break;
        }

        g_profile.negamaxCalls += 1;
        g_profile.negamaxMs += msSince(start);
        return best;
    }
}

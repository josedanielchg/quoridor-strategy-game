#include "heuristic/HeuristicEngine.hpp"
#include "game/GameRules.hpp"

#include <chrono>
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

        int evaluateTerminal(const GameState &state, int rootPlayerId, int depth)
        {
            if (state.winnerId == 0)
                return evaluateState(const_cast<GameState &>(state), rootPlayerId);

            if (state.winnerId == rootPlayerId)
                return kWinScore + depth;
            return -kWinScore - depth;
        }

        int alphaBeta(GameState &state, int depth, int alpha, int beta, SearchContext &ctx)
        {
            if (ctx.timeUp)
                return evaluateState(state, ctx.rootPlayerId);

            if (std::chrono::steady_clock::now() >= ctx.deadline)
            {
                ctx.timeUp = true;
                return evaluateState(state, ctx.rootPlayerId);
            }

            ctx.nodes++;

            if (depth <= 0 || state.winnerId != 0)
                return evaluateTerminal(state, ctx.rootPlayerId, depth);

            const uint64_t key = computeZobrist(state);
            if (ctx.tt)
            {
                if (const TTEntry *entry = ctx.tt->probe(key))
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

            std::vector<Move> moves = generateLegalMoves(state);
            if (moves.empty())
                return evaluateTerminal(state, ctx.rootPlayerId, depth);

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
        std::vector<Move> moves = generateLegalMoves(working);
        if (moves.empty())
        {
            int playerId = state.currentPlayerId;
            int idx = (playerId == 2) ? 1 : 0;
            return Move::Pawn(state.pawnX[idx], state.pawnY[idx], playerId);
        }

        m_tt.clear();

        SearchContext ctx{};
        ctx.config = &m_config;
        ctx.tt = &m_tt;
        ctx.rootPlayerId = state.currentPlayerId;
        ctx.deadline = std::chrono::steady_clock::now() +
                       std::chrono::milliseconds(m_config.timeLimitMs);

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

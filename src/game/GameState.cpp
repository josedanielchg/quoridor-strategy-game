#include "game/GameState.hpp"
#include "ai/PathFinder.hpp"
#include "game/Wall.hpp"
#include <array>
#include <chrono>
#include <iostream>

namespace Game
{
    GameState::GameState()
        : m_pawns{PawnState{4, Board::SIZE - 1}, PawnState{4, 0}},
          m_walls(),
          m_wallsRemaining{MAX_WALLS_PER_PLAYER, MAX_WALLS_PER_PLAYER},
          m_currentPlayer(1),
          m_winner(0)
    {
    }

    GameState::GameState(const Board &board, int currentPlayer, int p1WallsRemaining, int p2WallsRemaining)
        : m_pawns{PawnState{4, Board::SIZE - 1}, PawnState{4, 0}},
          m_walls(),
          m_wallsRemaining{p1WallsRemaining, p2WallsRemaining},
          m_currentPlayer(currentPlayer),
          m_winner(0)
    {
        const Pawn *p1 = board.getPawnById(1);
        const Pawn *p2 = board.getPawnById(2);

        if (p1)
            m_pawns[0] = {p1->x(), p1->y()};
        if (p2)
            m_pawns[1] = {p2->x(), p2->y()};

        for (const auto &wall : board.getAllWalls())
        {
            m_walls.push_back({wall.x(), wall.y(), wall.orientation()});
        }
    }

    int GameState::currentPlayer() const { return m_currentPlayer; }
    int GameState::winner() const { return m_winner; }
    bool GameState::isGameOver() const { return m_winner != 0; }

    int GameState::wallsRemaining(int playerId) const
    {
        if (playerId == 1)
            return m_wallsRemaining[0];
        if (playerId == 2)
            return m_wallsRemaining[1];
        return 0;
    }

    PawnState GameState::pawn(int playerId) const
    {
        if (playerId == 1)
            return m_pawns[0];
        return m_pawns[1];
    }

    const std::vector<WallState> &GameState::walls() const { return m_walls; }

    bool GameState::buildBoard(Board &board) const
    {
        board.init(false);

        board.setPawns(m_pawns[0].x, m_pawns[0].y, m_pawns[1].x, m_pawns[1].y);

        for (const auto &wall : m_walls)
        {
            if (!board.placeWall(wall.x, wall.y, wall.orientation))
                return false;
        }

        return true;
    }

    bool GameState::applyMove(const Move &move)
    {
        int playerId = move.playerId();
        if (playerId == 0)
        {
            playerId = m_currentPlayer;
        }
        else if (playerId != m_currentPlayer)
        {
            return false;
        }

        Board board;
        if (!buildBoard(board))
            return false;

        bool success = false;

        if (move.type() == MoveType::PawnMove)
        {
            success = board.movePawn(playerId, move.x(), move.y());
            if (success)
            {
                m_pawns[playerId - 1] = {move.x(), move.y()};
            }
        }
        else if (move.type() == MoveType::WallPlacement)
        {
            int idx = playerId - 1;
            if (m_wallsRemaining[idx] <= 0)
                return false;

            Wall tempWall(move.x(), move.y(), move.orientation());
            if (!tempWall.isValidMove(board, move.x(), move.y()))
                return false;

            board.toggleWall(move.x(), move.y(), move.orientation(), true);

            const Pawn *p1 = board.getPawnById(1);
            const Pawn *p2 = board.getPawnById(2);

            bool p1HasPath = p1 && PathFinder::doesPathExistBFS(board, p1->x(), p1->y(), 0);
            bool p2HasPath = p2 && PathFinder::doesPathExistBFS(board, p2->x(), p2->y(), Board::SIZE - 1);

            board.toggleWall(move.x(), move.y(), move.orientation(), false);

            if (!p1HasPath || !p2HasPath)
                return false;

            success = true;
            m_walls.push_back({move.x(), move.y(), move.orientation()});
            m_wallsRemaining[idx] -= 1;
        }

        if (success)
        {
            if (hasPlayerWon(playerId))
                m_winner = playerId;
            m_currentPlayer = (playerId == 1) ? 2 : 1;
        }

        return success;
    }

    bool GameState::applyMoveUnchecked(const Move &move)
    {
        int playerId = move.playerId();
        if (playerId == 0)
            playerId = m_currentPlayer;

        if (playerId != m_currentPlayer)
            return false;

        bool success = false;

        if (move.type() == MoveType::PawnMove)
        {
            m_pawns[playerId - 1] = {move.x(), move.y()};
            success = true;
        }
        else if (move.type() == MoveType::WallPlacement)
        {
            int idx = playerId - 1;
            if (m_wallsRemaining[idx] <= 0)
                return false;

            m_walls.push_back({move.x(), move.y(), move.orientation()});
            m_wallsRemaining[idx] -= 1;
            success = true;
        }

        if (success)
        {
            if (hasPlayerWon(playerId))
                m_winner = playerId;
            m_currentPlayer = (playerId == 1) ? 2 : 1;
        }

        return success;
    }

    bool GameState::hasPlayerWon(int playerId) const
    {
        if (playerId == 1)
            return m_pawns[0].y == 0;
        if (playerId == 2)
            return m_pawns[1].y == Board::SIZE - 1;
        return false;
    }

    std::vector<Move> GameState::generateLegalMoves() const
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<Move> moves;

        Board board;
        if (!buildBoard(board))
            return moves;

        const Pawn *pawn = board.getPawnById(m_currentPlayer);
        if (!pawn)
            return moves;

        for (int y = 0; y < Board::SIZE; ++y)
        {
            for (int x = 0; x < Board::SIZE; ++x)
            {
                if (pawn->isValidMove(board, x, y))
                {
                    moves.push_back(Move::Pawn(x, y, m_currentPlayer));
                }
            }
        }

        int wallIdx = m_currentPlayer - 1;
        if (m_wallsRemaining[wallIdx] <= 0)
            return moves;

        const Pawn *p1 = board.getPawnById(1);
        const Pawn *p2 = board.getPawnById(2);
        if (!p1 || !p2)
            return moves;

        const int radius = 2;
        const int maxWallCoord = Board::SIZE - 2;
        std::array<bool, (Board::SIZE - 1) * (Board::SIZE - 1) * 2> seen{};
        int wallCandidates = 0;
        int wallAccepted = 0;

        auto markSeen = [&](int x, int y, Orientation o) -> bool {
            int oIdx = (o == Orientation::Horizontal) ? 0 : 1;
            int idx = (y * (Board::SIZE - 1) + x) * 2 + oIdx;
            if (seen[idx])
                return false;
            seen[idx] = true;
            return true;
        };

        auto addCandidate = [&](int x, int y, Orientation o) {
            if (x < 0 || y < 0 || x > maxWallCoord || y > maxWallCoord)
                return;
            if (!markSeen(x, y, o))
                return;

            wallCandidates += 1;
            Wall tempWall(x, y, o);
            if (!tempWall.isValidMove(board, x, y))
                return;

            board.toggleWall(x, y, o, true);
            bool p1HasPath = PathFinder::doesPathExistBFS(board, p1->x(), p1->y(), 0);
            bool p2HasPath = PathFinder::doesPathExistBFS(board, p2->x(), p2->y(), Board::SIZE - 1);
            board.toggleWall(x, y, o, false);

            if (p1HasPath && p2HasPath)
            {
                moves.push_back(Move::Wall(x, y, o, m_currentPlayer));
                wallAccepted += 1;
            }
        };

        const Pawn *focusPawn = (m_currentPlayer == 1) ? p2 : p1;
        for (int y = focusPawn->y() - radius; y <= focusPawn->y() + radius; ++y)
        {
            for (int x = focusPawn->x() - radius; x <= focusPawn->x() + radius; ++x)
            {
                for (Orientation o : {Orientation::Horizontal, Orientation::Vertical})
                {
                    addCandidate(x, y, o);
                }
            }
        }

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::high_resolution_clock::now() - start)
                           .count();
        std::cout << "[AI] generateLegalMoves " << elapsed << " ms | "
                  << "moves=" << moves.size() << ", walls=" << wallAccepted
                  << ", candidates=" << wallCandidates << "\n";

        return moves;
    }

    std::vector<Move> GameState::generatePawnMoves() const
    {
        std::vector<Move> moves;

        Board board;
        if (!buildBoard(board))
            return moves;

        const Pawn *pawn = board.getPawnById(m_currentPlayer);
        if (!pawn)
            return moves;

        for (int y = 0; y < Board::SIZE; ++y)
        {
            for (int x = 0; x < Board::SIZE; ++x)
            {
                if (pawn->isValidMove(board, x, y))
                {
                    moves.push_back(Move::Pawn(x, y, m_currentPlayer));
                }
            }
        }

        return moves;
    }

    bool GameState::syncBoard(Board &board) const
    {
        return buildBoard(board);
    }
}

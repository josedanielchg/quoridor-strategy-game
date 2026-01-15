#include "game/GameState.hpp"

namespace Game
{
    GameState::GameState()
        : m_pawns{PawnState{4, Board::SIZE - 1}, PawnState{4, 0}},
          m_walls(),
          m_wallsRemaining{MAX_WALLS_PER_PLAYER, MAX_WALLS_PER_PLAYER},
          m_currentPlayer(1)
    {
    }

    GameState::GameState(const Board &board, int currentPlayer, int p1WallsRemaining, int p2WallsRemaining)
        : m_pawns{PawnState{4, Board::SIZE - 1}, PawnState{4, 0}},
          m_walls(),
          m_wallsRemaining{p1WallsRemaining, p2WallsRemaining},
          m_currentPlayer(currentPlayer)
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
        board.init();

        Pawn *p1 = board.getPawnById(1);
        Pawn *p2 = board.getPawnById(2);

        if (p1)
            p1->setPosition(m_pawns[0].x, m_pawns[0].y);
        if (p2)
            p2->setPosition(m_pawns[1].x, m_pawns[1].y);

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

            success = board.placeWall(move.x(), move.y(), move.orientation());
            if (success)
            {
                m_walls.push_back({move.x(), move.y(), move.orientation()});
                m_wallsRemaining[idx] -= 1;
            }
        }

        if (success)
        {
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
        if (m_wallsRemaining[wallIdx] > 0)
        {
            for (int y = 0; y < Board::SIZE - 1; ++y)
            {
                for (int x = 0; x < Board::SIZE - 1; ++x)
                {
                    for (Orientation orientation : {Orientation::Horizontal, Orientation::Vertical})
                    {
                        Board tempBoard;
                        if (!buildBoard(tempBoard))
                            continue;

                        if (tempBoard.placeWall(x, y, orientation))
                        {
                            moves.push_back(Move::Wall(x, y, orientation, m_currentPlayer));
                        }
                    }
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

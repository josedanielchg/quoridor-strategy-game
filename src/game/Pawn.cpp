#include "game/Pawn.hpp"
#include "game/Board.hpp"
#include "game/GameRules.hpp"
#include <cstring>

namespace Game
{
    namespace
    {
        bool fillStateFromBoard(const Board &board, GameState &state)
        {
            std::memset(&state, 0, sizeof(state));

            const Pawn *p1 = board.getPawnById(1);
            const Pawn *p2 = board.getPawnById(2);
            if (!p1 || !p2)
                return false;

            state.pawnX[0] = static_cast<uint8_t>(p1->x());
            state.pawnY[0] = static_cast<uint8_t>(p1->y());
            state.pawnX[1] = static_cast<uint8_t>(p2->x());
            state.pawnY[1] = static_cast<uint8_t>(p2->y());
            state.wallsRemaining[0] = GameState::MAX_WALLS_PER_PLAYER;
            state.wallsRemaining[1] = GameState::MAX_WALLS_PER_PLAYER;
            state.distDirty = 1;
            state.currentPlayerId = 1;
            state.winnerId = 0;

            for (const auto &wall : board.getAllWalls())
            {
                int x = wall.x();
                int y = wall.y();
                if (wall.orientation() == Orientation::Horizontal)
                {
                    if (x >= 0 && x < GameState::WALL_GRID && y >= 0 && y < GameState::WALL_GRID)
                        state.hWalls[x][y] = 1;
                }
                else
                {
                    if (x >= 0 && x < GameState::WALL_GRID && y >= 0 && y < GameState::WALL_GRID)
                        state.vWalls[x][y] = 1;
                }
            }

            return true;
        }
    }

    Pawn::Pawn(int id, int startX, int startY)
        : VisualEntity(startX, startY), m_id(id)
    {
        (void)initSprite();
    }

    int Pawn::id() const { return m_id; }

    std::string Pawn::getTexturePath() const
    {
        if (m_id == 1)
            return "assets/textures/pawn_p1.png";
        return "assets/textures/pawn_p2.png";
    }

    sf::Vector2f Pawn::getSpriteOrigin(const sf::Vector2u &texSize) const
    {
        // Hardcoded values to preserve previous visual tweaks
        return {112.f / 2.f, 165.f - 20.f};
    }

    bool Pawn::isValidMove(const Board &board, int targetX, int targetY) const
    {
        GameState state{};
        if (!fillStateFromBoard(board, state))
            return false;

        return isPawnMoveValid(state, m_id, targetX, targetY);
    }
}

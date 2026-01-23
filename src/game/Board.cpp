#include "game/Board.hpp"
#include "game/GameState.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace Game
{
    static const sf::Vector2f BACKGROUND_CANVAS_SIZE = {1728.f, 1117.f};

    Board::Board()
        : m_backgroundSprite(m_backgroundTexture)
    {
        init();
    }

    void Board::init(bool loadVisuals)
    {
        m_fields.clear();
        m_fields.reserve(SIZE * SIZE);
        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
            {
                m_fields.emplace_back(x, y);
            }
        }
        m_walls.clear();

        if (loadVisuals && !m_hasBackground)
        {
            if (!m_backgroundTexture.loadFromFile("assets/textures/bg.png"))
            {
                std::cerr << "Error: Could not load bg.png\n";
                m_hasBackground = false;
            }
            else
            {
                m_backgroundSprite.setTexture(m_backgroundTexture, true);
                sf::Vector2u texSize = m_backgroundTexture.getSize();
                m_backgroundSprite.setOrigin({float(texSize.x) / 2.f, float(texSize.y) / 2.f});
                m_backgroundSprite.setPosition({0.f, 0.f});
                m_hasBackground = true;
            }
        }
    }

    Field &Board::getField(int x, int y)
    {
        if (!isValid(x, y))
            throw std::out_of_range("Out of bounds");
        return m_fields[y * SIZE + x];
    }

    const Field &Board::getField(int x, int y) const
    {
        if (!isValid(x, y))
            throw std::out_of_range("Out of bounds");
        return m_fields[y * SIZE + x];
    }

    const std::vector<Field> &Board::getAllFields() const { return m_fields; }
    const std::vector<Wall> &Board::getAllWalls() const { return m_walls; }
    const std::vector<Pawn> &Board::getAllPawns() const { return m_pawns; }

    void Board::setPawns(int p1x, int p1y, int p2x, int p2y)
    {
        m_pawns.clear();
        m_pawns.emplace_back(1, p1x, p1y);
        m_pawns.emplace_back(2, p2x, p2y);
    }

    bool Board::loadFromState(const GameState &state)
    {
        init(false);
        setPawns(state.pawnX[0], state.pawnY[0], state.pawnX[1], state.pawnY[1]);

        for (int y = 0; y < GameState::WALL_GRID; ++y)
        {
            for (int x = 0; x < GameState::WALL_GRID; ++x)
            {
                if (state.hWalls[x][y])
                {
                    if (!placeWall(x, y, Orientation::Horizontal))
                        return false;
                }
                if (state.vWalls[x][y])
                {
                    if (!placeWall(x, y, Orientation::Vertical))
                        return false;
                }
            }
        }

        return true;
    }

    void Board::drawBackground(sf::RenderWindow &window) const
    {
        if (!m_hasBackground)
            return;

        sf::Sprite sprite = m_backgroundSprite;
        const sf::View view = window.getView();
        const sf::Vector2f viewSize = view.getSize();
        const sf::Vector2f viewCenter = view.getCenter();
        const sf::Vector2u texSize = m_backgroundTexture.getSize();
        if (texSize.x == 0 || texSize.y == 0)
            return;

        const float scale = std::max(viewSize.x / BACKGROUND_CANVAS_SIZE.x,
                                     viewSize.y / BACKGROUND_CANVAS_SIZE.y);
        const float designToTextureScale = BACKGROUND_CANVAS_SIZE.x / float(texSize.x);
        const float uniformScale = scale * designToTextureScale;
        sprite.setScale({uniformScale, uniformScale});
        sprite.setPosition(viewCenter);
        window.draw(sprite);
    }

    const Pawn *Board::getPawnAt(int x, int y) const
    {
        for (const auto &p : m_pawns)
        {
            if (p.x() == x && p.y() == y)
                return &p;
        }
        return nullptr;
    }

    Pawn *Board::getPawnById(int id)
    {
        for (auto &p : m_pawns)
        {
            if (p.id() == id)
                return &p;
        }
        return nullptr;
    }

    const Pawn *Board::getPawnById(int id) const
    {
        for (const auto &p : m_pawns)
        {
            if (p.id() == id)
                return &p;
        }
        return nullptr;
    }

    bool Board::placeWall(int x, int y, Orientation orientation)
    {
        // Create temp wall to check validity
        Wall tempWall(x, y, orientation);

        if (!tempWall.isValidMove(*this, x, y))
            return false;

        toggleWall(x, y, orientation, true);
        m_walls.emplace_back(x, y, orientation);
        return true;
    }
    void Board::toggleWall(int x, int y, Orientation ori, bool blocking)
    {
        if (ori == Orientation::Horizontal)
        {
            if (blocking)
            {
                getField(x, y).disconnect(Direction::Down);
                getField(x, y + 1).disconnect(Direction::Up);
                getField(x + 1, y).disconnect(Direction::Down);
                getField(x + 1, y + 1).disconnect(Direction::Up);
            }
            else
            {
                getField(x, y).connect(Direction::Down);
                getField(x, y + 1).connect(Direction::Up);
                getField(x + 1, y).connect(Direction::Down);
                getField(x + 1, y + 1).connect(Direction::Up);
            }
        }
        else
        {
            if (blocking)
            {
                getField(x, y).disconnect(Direction::Right);
                getField(x + 1, y).disconnect(Direction::Left);
                getField(x, y + 1).disconnect(Direction::Right);
                getField(x + 1, y + 1).disconnect(Direction::Left);
            }
            else
            {
                getField(x, y).connect(Direction::Right);
                getField(x + 1, y).connect(Direction::Left);
                getField(x, y + 1).connect(Direction::Right);
                getField(x + 1, y + 1).connect(Direction::Left);
            }
        }
    }

    bool Board::movePawn(int pawnId, int targetX, int targetY)
    {
        Pawn *pawn = getPawnById(pawnId);
        if (!pawn)
            return false;

        // Ask the Pawn class if the move is valid
        if (pawn->isValidMove(*this, targetX, targetY))
        {
            pawn->setPosition(targetX, targetY);
            return true;
        }
        return false;
    }

    bool Board::isValid(int x, int y) const
    {
        return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
    }

    // Win condition now lives in GameState.
}

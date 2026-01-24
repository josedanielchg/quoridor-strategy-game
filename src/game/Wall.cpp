#include "game/Wall.hpp"
#include "game/Board.hpp"
#include "game/WallRules.hpp"
#include "resources/ResourceLoader.hpp"
#include <cstring>

namespace Game
{
    namespace
    {
        void buildWallGrids(const Board &board,
                            uint8_t hWalls[WALL_GRID][WALL_GRID],
                            uint8_t vWalls[WALL_GRID][WALL_GRID])
        {
            std::memset(hWalls, 0, sizeof(uint8_t) * WALL_GRID * WALL_GRID);
            std::memset(vWalls, 0, sizeof(uint8_t) * WALL_GRID * WALL_GRID);

            for (const auto &wall : board.getAllWalls())
            {
                int x = wall.x();
                int y = wall.y();
                if (x < 0 || x >= WALL_GRID || y < 0 || y >= WALL_GRID)
                    continue;
                if (wall.orientation() == Orientation::Horizontal)
                    hWalls[x][y] = 1;
                else
                    vWalls[x][y] = 1;
            }
        }
    }

    Wall::Wall(int x, int y, Orientation orientation)
        : VisualEntity(x, y),
          m_orientation(orientation),
          m_texDown(),
          m_spriteDown(m_texDown)
    {
        (void)initSprite();

        Resources::loadTextureInto(m_texDown,
                                   "assets/textures/wall-down.png",
                                   "Wall",
                                   "Wall down texture");
        m_spriteDown.setTexture(m_texDown, true);
        sf::Vector2u downSize = m_texDown.getSize();
        m_spriteDown.setOrigin({float(downSize.x) / 1.09f, float(downSize.y) / 1.29f});
    }

    Wall::Wall(Wall &&other) noexcept
        : VisualEntity(std::move(other)),
          m_orientation(other.m_orientation),
          m_texDown(std::move(other.m_texDown)),
          m_spriteDown(std::move(other.m_spriteDown))
    {
        m_spriteDown.setTexture(m_texDown, false);
    }

    Wall &Wall::operator=(Wall &&other) noexcept
    {
        if (this != &other)
        {
            VisualEntity::operator=(std::move(other));
            m_orientation = other.m_orientation;
            m_texDown = std::move(other.m_texDown);
            m_spriteDown = std::move(other.m_spriteDown);
            m_spriteDown.setTexture(m_texDown, false);
        }
        return *this;
    }

    Orientation Wall::orientation() const { return m_orientation; }

    const sf::Sprite &Wall::upperSprite() const { return sprite(); }

    const sf::Sprite &Wall::downSprite() const { return m_spriteDown; }

    const Wall &Wall::previewWall()
    {
        static Wall wall(0, 0, Orientation::Horizontal);
        return wall;
    }

    const sf::Sprite &Wall::previewUpperSprite()
    {
        return previewWall().upperSprite();
    }

    const sf::Sprite &Wall::previewDownSprite()
    {
        return previewWall().downSprite();
    }

    std::string Wall::getTexturePath() const
    {
        return "assets/textures/wall-upper.png";
    }

    sf::Vector2f Wall::getSpriteOrigin(const sf::Vector2u &texSize) const
    {
        return {float(texSize.x) / 1.09f, float(texSize.y) / 1.29f};
    }

    bool Wall::isValidMove(const Board &board, int targetX, int targetY) const
    {
        uint8_t hWalls[WALL_GRID][WALL_GRID];
        uint8_t vWalls[WALL_GRID][WALL_GRID];
        buildWallGrids(board, hWalls, vWalls);
        return isWallPlacementLegalLocal(hWalls, vWalls, targetX, targetY, m_orientation);
    }
}

#include "ui/GameRenderer.hpp"
#include "ui/ViewUtils.hpp"
#include <iostream>
#include <cmath>

namespace UI
{

    static const sf::Vector2f REFERENCE_SIZE = UI_DESIGN_SIZE;

    static const float ISO_WIDTH = UI::FIELD_SIZE.x * UI::BOARD_SCALE * 0.50f;
    static const float ISO_HEIGHT = UI::FIELD_SIZE.y * UI::BOARD_SCALE * 0.36f;

    GameRenderer::GameRenderer()
        : m_spriteTile(m_texTile),
          m_isoWidth(ISO_WIDTH),
          m_isoHeight(ISO_HEIGHT)
    {
        // 1. Calculate the full height of the board in pixels
        float boardGeoHeight = (8 + 8) * m_isoHeight;

        // 2. Find the vertical center of the board geometry
        float boardCenterY = boardGeoHeight / 2.f;

        // 3. Align Board Center with Screen Center
        float originY = (REFERENCE_SIZE.y / 2.f) - boardCenterY;

        // 4. Horizontal Center is simple
        float originX = REFERENCE_SIZE.x / 2.f;

        // 5. Visual Tweak: Move it slightly UP (-50.f) because Pawns stick up
        m_boardOrigin = {originX, originY - 30.f * UI::BOARD_SCALE};
    }

    bool GameRenderer::init()
    {
        // 1. Load Tile Texture (tiles are renderer-owned)
        if (!m_texTile.loadFromFile("assets/textures/tile.png"))
        {
            std::cerr << "Error: Could not load tile.png\n";
            return false;
        }

        // Setup Tile Sprite
        m_spriteTile.setTexture(m_texTile, true);
        sf::Vector2u tSize = m_texTile.getSize();
        m_spriteTile.setOrigin({float(tSize.x) / 2.f, float(tSize.y) / 2.f});
        if (tSize.x > 0 && tSize.y > 0)
        {
            m_spriteTile.setScale({(UI::FIELD_SIZE.x * UI::BOARD_SCALE) / float(tSize.x),
                                   (UI::FIELD_SIZE.y * UI::BOARD_SCALE) / float(tSize.y)});
        }

        return true;
    }

    // Converts Grid Coordinates (0..8) to Screen Pixels
    sf::Vector2f GameRenderer::cartesianToIsometric(int gridX, int gridY) const
    {
        float x = (gridX - gridY) * m_isoWidth;
        float y = (gridX + gridY) * m_isoHeight;
        return {x + m_boardOrigin.x, y + m_boardOrigin.y};
    }

    void GameRenderer::applyWallTransform(sf::Sprite &sprite, sf::Vector2f &pos, Game::Orientation orientation) const
    {
        sprite.setRotation(sf::degrees(0));
        sprite.setScale({1.f, 1.f});

        if (orientation == Game::Orientation::Horizontal)
        {
            pos.y += m_isoHeight * 0.1f;
        }
        else
        {
            sprite.setScale({-1.f, 1.f});
        }
    }

    void GameRenderer::drawWallPart(sf::RenderWindow &window, sf::Vector2i gridPos, Game::Orientation orientation, const sf::Sprite &baseSprite, bool isPreview)
    {
        sf::Vector2f pos = cartesianToIsometric(gridPos.x, gridPos.y);
        sf::Sprite s = baseSprite;
        applyWallTransform(s, pos, orientation);

        const sf::Texture &texture = s.getTexture();
        const sf::Vector2u texSize = texture.getSize();
        if (texSize.x > 0 && texSize.y > 0)
        {
            const sf::Vector2f currentScale = s.getScale();
            const sf::Vector2f targetScale = {(UI::WALL_SIZE.x * UI::BOARD_SCALE) / float(texSize.x),
                                              (UI::WALL_SIZE.y * UI::BOARD_SCALE) / float(texSize.y)};
            s.setScale({currentScale.x * targetScale.x,
                        currentScale.y * targetScale.y});
        }

        s.setPosition(pos);
        s.setColor(isPreview ? sf::Color(255, 255, 255, 128) : sf::Color(255, 255, 255));
        window.draw(s);
    }

    void GameRenderer::drawTile(sf::RenderWindow &window, int gridX, int gridY)
    {
        sf::Vector2f pos = cartesianToIsometric(gridX, gridY);

        if (gridX == m_hoveredCoords.x && gridY == m_hoveredCoords.y && !m_showWallPreview)
            m_spriteTile.setColor(sf::Color::White);
        else
            m_spriteTile.setColor(sf::Color(200, 200, 200));

        m_spriteTile.setPosition(pos);
        window.draw(m_spriteTile);
    }

    void GameRenderer::drawPawnAt(sf::RenderWindow &window, const std::vector<Game::Pawn> &pawns, int gridX, int gridY)
    {
        for (const auto &pawn : pawns)
        {
            if (pawn.x() != gridX || pawn.y() != gridY)
                continue;

            sf::Vector2f pos = cartesianToIsometric(pawn.x(), pawn.y());

            sf::Sprite s = pawn.sprite();
            const sf::Texture &texture = s.getTexture();
            const sf::Vector2u texSize = texture.getSize();
            if (texSize.x > 0 && texSize.y > 0)
            {
                const sf::Vector2f targetSize = pawn.id() == 1 ? UI::PAWN_P1_SIZE
                                                               : UI::PAWN_P2_SIZE;
                s.setScale({(targetSize.x * UI::BOARD_SCALE) / float(texSize.x),
                            (targetSize.y * UI::BOARD_SCALE) / float(texSize.y)});
            }
            s.setColor(sf::Color::White);
            s.setPosition(pos);
            window.draw(s);
        }
    }

    void GameRenderer::drawWallsAt(sf::RenderWindow &window, const std::vector<Game::Wall> &walls, int gridX, int gridY)
    {
        for (const auto &wall : walls)
        {
            if (wall.x() == gridX && wall.y() == gridY)
            {
                drawWallPart(window, {gridX, gridY}, wall.orientation(), wall.upperSprite(), false);
                continue;
            }

            if (wall.orientation() == Game::Orientation::Vertical)
            {
                if (wall.x() == gridX && wall.y() + 1 == gridY)
                    drawWallPart(window, {gridX, gridY}, wall.orientation(), wall.downSprite(), false);
            }
            else
            {
                if (wall.x() + 1 == gridX && wall.y() == gridY)
                    drawWallPart(window, {gridX, gridY}, wall.orientation(), wall.downSprite(), false);
            }
        }

        if (m_showWallPreview && m_previewWallPos.x != -1)
        {
            if (m_previewWallPos.x == gridX && m_previewWallPos.y == gridY)
            {
                drawWallPart(window, {gridX, gridY}, m_previewWallOri, Game::Wall::previewUpperSprite(), true);
            }
            else if (m_previewWallOri == Game::Orientation::Vertical)
            {
                if (m_previewWallPos.x == gridX && m_previewWallPos.y + 1 == gridY)
                    drawWallPart(window, {gridX, gridY}, m_previewWallOri, Game::Wall::previewDownSprite(), true);
            }
            else
            {
                if (m_previewWallPos.x + 1 == gridX && m_previewWallPos.y == gridY)
                    drawWallPart(window, {gridX, gridY}, m_previewWallOri, Game::Wall::previewDownSprite(), true);
            }
        }
    }

    void GameRenderer::render(sf::RenderWindow &window, const Game::Board &board)
    {
        window.setView(m_view);

        board.drawBackground(window);

        const auto &pawns = board.getAllPawns();
        const auto &walls = board.getAllWalls();

        for (int row = 0; row < Game::Board::SIZE; ++row)
        {
            for (int x = 0; x < Game::Board::SIZE; ++x)
            {
                drawTile(window, x, row);
                drawPawnAt(window, pawns, x, row);
                drawWallsAt(window, walls, x, row);
            }
        }
    }

    void GameRenderer::handleResize(sf::RenderWindow &window, sf::Vector2u size)
    {
        (void)window;
        m_view = UI::makeLetterboxView(size, REFERENCE_SIZE);
    }

    sf::Vector2i GameRenderer::getMouseGridPos(const sf::RenderWindow &window, sf::Vector2i mousePos) const
    {
        // 1. Convert Screen Pixels -> World Coords (taking View/Zoom into account)
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, m_view);

        // 2. Remove the Board Origin Offset
        float localX = worldPos.x - m_boardOrigin.x;
        float localY = worldPos.y - m_boardOrigin.y;

        // 3. Inverse Isometric Formula
        float approxX = (localX / m_isoWidth + localY / m_isoHeight) / 2.f;
        float approxY = (localY / m_isoHeight - localX / m_isoWidth) / 2.f;

        // 4. Round to nearest integer index
        int gridX = static_cast<int>(std::round(approxX));
        int gridY = static_cast<int>(std::round(approxY));

        // 5. Bounds Check (Quoridor is 9x9, indices 0..8)
        if (gridX >= 0 && gridX < 9 && gridY >= 0 && gridY < 9)
        {
            return {gridX, gridY};
        }

        return {-1, -1};
    }

    void GameRenderer::setHoveredTile(sf::Vector2i gridCoords)
    {
        m_hoveredCoords = gridCoords;
    }

    void GameRenderer::setWallPreview(bool active, sf::Vector2i gridPos, Game::Orientation orientation)
    {
        m_showWallPreview = active;
        m_previewWallPos = gridPos;
        m_previewWallOri = orientation;
    }
}

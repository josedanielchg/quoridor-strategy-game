#include "ui/GameRenderer.hpp"
#include <iostream>
#include <cmath>

namespace UI
{

    static const sf::Vector2f REFERENCE_SIZE = {2200.f, 1400.f};

    static const float ISO_WIDTH = 112.f;
    static const float ISO_HEIGHT = 57.f;

    GameRenderer::GameRenderer()
        : m_spriteTile(m_texTile), m_spritePawn(m_texPawnP1), m_spriteWall(m_texWall), m_isoWidth(ISO_WIDTH), m_isoHeight(ISO_HEIGHT)
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
        m_boardOrigin = {originX, originY - 30.f};
    }

    bool GameRenderer::init()
    {
        // 1. Load Textures
        if (!m_texTile.loadFromFile("assets/textures/tile.png"))
        {
            std::cerr << "Error: Could not load tile.png\n";
            return false;
        }
        if (!m_texPawnP1.loadFromFile("assets/textures/pawn_p1.png"))
        {
            std::cerr << "Error: Could not load pawn_p1.png\n";
            return false;
        }
        if (!m_texPawnP2.loadFromFile("assets/textures/pawn_p2.png"))
        {
            std::cerr << "Error: Could not load pawn_p2.png\n";
            return false;
        }
        if (!m_texWall.loadFromFile("assets/textures/wall.png"))
        {
            std::cerr << "Error: Could not load wall.png\n";
            return false;
        }

        // 2. Setup Tile Sprite
        m_spriteTile.setTexture(m_texTile, true);

        sf::Vector2u tSize = m_texTile.getSize();
        m_spriteTile.setOrigin({float(tSize.x) / 2.f, float(tSize.y) / 2.f});

        // 3. Setup Pawn Sprite
        m_spritePawn.setTexture(m_texPawnP1, true);
        m_spritePawn.setOrigin({112.f / 2.f, 165.f - 20.f});

        m_spriteWall.setTexture(m_texWall, true);
        sf::Vector2u wSize = m_texWall.getSize();
        m_spriteWall.setOrigin({float(wSize.x) / 2.f, float(wSize.y) / 2.f});

        return true;
    }

    // Converts Grid Coordinates (0..8) to Screen Pixels
    sf::Vector2f GameRenderer::cartesianToIsometric(int gridX, int gridY) const
    {
        float x = (gridX - gridY) * m_isoWidth;
        float y = (gridX + gridY) * m_isoHeight;
        return {x + m_boardOrigin.x, y + m_boardOrigin.y};
    }

    void GameRenderer::render(sf::RenderWindow &window, const Game::Board &board)
    {
        window.setView(m_view);

        // 1. Draw Fields
        const auto &fields = board.getAllFields();
        for (const auto &field : fields)
        {
            sf::Vector2f pos = cartesianToIsometric(field.x(), field.y());

            // Hover effect
            if (field.x() == m_hoveredCoords.x && field.y() == m_hoveredCoords.y && !m_showWallPreview)
            {
                m_spriteTile.setColor(sf::Color::White);
            }
            else
            {
                m_spriteTile.setColor(sf::Color(200, 200, 200));
            }

            m_spriteTile.setPosition(pos);
            window.draw(m_spriteTile);
        }

        // 2. Draw Pawns
        const auto &pawns = board.getAllPawns();
        for (const auto &pawn : pawns)
        {
            sf::Vector2f pos = cartesianToIsometric(pawn.x(), pawn.y());

            // Determine texture based on ID
            if (pawn.id() == 1)
                m_spritePawn.setTexture(m_texPawnP1, true);
            else
                m_spritePawn.setTexture(m_texPawnP2, true);

            m_spritePawn.setColor(sf::Color::White);
            m_spritePawn.setPosition(pos);
            window.draw(m_spritePawn);
        }

        // 3. Walls (Helper & Loop) ... (Keep existing wall logic)
        auto drawWall = [&](int x, int y, Game::Orientation ori, bool isPreview) {
            sf::Vector2f pos = cartesianToIsometric(x, y);

            // Reset Transforms
            m_spriteWall.setRotation(sf::degrees(0));
            m_spriteWall.setScale({1.f, 1.f});

            if (ori == Game::Orientation::Horizontal) {
                pos.y += m_isoHeight * 0.1f; 
            } 
            else {
                // Flip for vertical orientation
                m_spriteWall.setScale({-1.f, 1.f}); 
            }
            
            m_spriteWall.setPosition(pos);

            if (isPreview) {
                m_spriteWall.setColor(sf::Color(255, 255, 255, 128)); 
            } else {
                m_spriteWall.setColor(sf::Color(255, 255, 255));
            }
            
            window.draw(m_spriteWall);
        };

        for (const auto &wall : board.getAllWalls())
        {
            drawWall(wall.x(), wall.y(), wall.orientation(), false);
        }

        if (m_showWallPreview && m_previewWallPos.x != -1)
        {
            drawWall(m_previewWallPos.x, m_previewWallPos.y, m_previewWallOri, true);
        }
    }

    void GameRenderer::handleResize(sf::RenderWindow &window, sf::Vector2u size)
    {
        m_view.setSize(REFERENCE_SIZE);
        m_view.setCenter(REFERENCE_SIZE / 2.f);

        float windowRatio = float(size.x) / float(size.y);
        float refRatio = REFERENCE_SIZE.x / REFERENCE_SIZE.y;

        // SFML 3.0 Rect Constructor: {{pos}, {size}}
        sf::FloatRect viewport({0.f, 0.f}, {1.f, 1.f});

        if (windowRatio > refRatio)
        {
            // Window is WIDER -> Black bars on sides
            float scale = refRatio / windowRatio;
            viewport.size.x = scale;
            viewport.position.x = (1.f - scale) / 2.f;
        }
        else
        {
            // Window is TALLER -> Black bars on top/bottom
            float scale = windowRatio / refRatio;
            viewport.size.y = scale;
            viewport.position.y = (1.f - scale) / 2.f;
        }

        m_view.setViewport(viewport);
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
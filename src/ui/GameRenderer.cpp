#include "ui/GameRenderer.hpp"
#include <iostream>
#include <cmath>

namespace UI {

    GameRenderer::GameRenderer()
        : m_spriteTile(m_texTile)     
        , m_spritePawn(m_texPawnP1) // Initialize with P1 texture by default
        , m_isoWidth(112.f) // From your specs
        , m_isoHeight(57.f) // From your specs
        , m_boardOrigin({400.f, 100.f}) // Initial guess, updated on resize
    {
    }

    bool GameRenderer::init() {
        // 1. Load Textures
        if (!m_texTile.loadFromFile("assets/textures/tile.png")) {
            std::cerr << "Error: Could not load tile.png\n";
            return false;
        }
        if (!m_texPawnP1.loadFromFile("assets/textures/pawn_p1.png")) {
            std::cerr << "Error: Could not load pawn_p1.png\n";
            return false;
        }
        if (!m_texPawnP2.loadFromFile("assets/textures/pawn_p2.png")) {
            std::cerr << "Error: Could not load pawn_p2.png\n";
            return false;
        }

        // 2. Setup Tile Sprite 
        m_spriteTile.setTexture(m_texTile, true); 

        sf::Vector2u tSize = m_texTile.getSize();
        m_spriteTile.setOrigin({float(tSize.x) / 2.f, float(tSize.y) / 2.f});

        // 3. Setup Pawn Sprite
        m_spritePawn.setTexture(m_texPawnP1, true);
        m_spritePawn.setOrigin({112.f / 2.f, 165.f - 15.f}); 

        return true;
    }

    // Converts Grid Coordinates (0..8) to Screen Pixels
    sf::Vector2f GameRenderer::cartesianToIsometric(int gridX, int gridY) const {
        float x = (gridX - gridY) * m_isoWidth;
        float y = (gridX + gridY) * m_isoHeight;
        return {x + m_boardOrigin.x, y + m_boardOrigin.y};
    }

    void GameRenderer::render(sf::RenderWindow& window, const Game::Board& board) {
        window.setView(m_view);

        const auto& fields = board.getAllFields();
        
        for (const auto& field : fields) {
            sf::Vector2f pos = cartesianToIsometric(field.x(), field.y());

            m_spriteTile.setPosition(pos);
            window.draw(m_spriteTile);

            int occupant = field.occupantId();
            if (occupant != 0) {
                // Swap the texture depending on the player
                if (occupant == 1) m_spritePawn.setTexture(m_texPawnP1, true);
                else m_spritePawn.setTexture(m_texPawnP2, true);

                m_spritePawn.setPosition(pos);
                window.draw(m_spritePawn);
            }
        }
    }

    void GameRenderer::handleResize(sf::RenderWindow& window, sf::Vector2u size) {
        m_view.setSize({float(size.x), float(size.y)});
        m_view.setCenter({float(size.x) / 2.f, float(size.y) / 2.f});
        m_boardOrigin = {float(size.x) / 2.f, float(size.y) / 4.f}; 
    }
}
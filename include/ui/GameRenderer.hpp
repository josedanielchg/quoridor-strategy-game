#pragma once
#include <SFML/Graphics.hpp>
#include "game/Board.hpp"
#include <string>

namespace UI
{
    class GameRenderer
    {
    private:
        sf::Vector2f cartesianToIsometric(int gridX, int gridY) const; // Grid to screen #
        void applyWallTransform(sf::Sprite &sprite, sf::Vector2f &pos, Game::Orientation orientation) const; // Orient wall #
        void drawWallPart(sf::RenderWindow &window, sf::Vector2i gridPos, Game::Orientation orientation, const sf::Sprite &baseSprite, bool isPreview); // Draw wall part #
        void drawTile(sf::RenderWindow &window, int gridX, int gridY); // Draw tile #
        void drawPawnAt(sf::RenderWindow &window, const std::vector<Game::Pawn> &pawns, int gridX, int gridY); // Draw pawn #
        void drawWallsAt(sf::RenderWindow &window, const std::vector<Game::Wall> &walls, int gridX, int gridY); // Draw walls #

        // Textures (Must stay alive while game runs)
        sf::Texture m_texTile;

        // Flyweight Sprite for tiles
        sf::Sprite m_spriteTile;
        sf::Vector2i m_hoveredCoords = {-1, -1};

        bool m_showWallPreview = false;
        sf::Vector2i m_previewWallPos = {0, 0};
        Game::Orientation m_previewWallOri = Game::Orientation::Horizontal;

        // Isometric Layout Config
        sf::Vector2f m_boardOrigin; // Screen center offset
        float m_isoWidth;           // 112.f
        float m_isoHeight;          // 57.f

        sf::View m_view;

    public:
        GameRenderer(); // Build renderer #
        bool init(); // Load textures #
        void render(sf::RenderWindow &window, const Game::Board &board); // Draw board #
        void handleResize(sf::RenderWindow &window, sf::Vector2u size); // Update view #
        void setHoveredTile(sf::Vector2i gridCoords); // Set hover tile #
        sf::Vector2i getMouseGridPos(const sf::RenderWindow &window, sf::Vector2i mousePos) const; // Mouse to grid #
        void setWallPreview(bool active, sf::Vector2i gridPos, Game::Orientation orientation); // Wall preview #
    };

}

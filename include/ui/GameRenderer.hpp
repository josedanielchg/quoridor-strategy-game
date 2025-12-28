#pragma once
#include <SFML/Graphics.hpp>
#include "game/Board.hpp"
#include <string>

namespace UI
{
    class GameRenderer
    {
        private:
            // Helper: Convert Grid(x,y) to Screen(px,py)
            sf::Vector2f cartesianToIsometric(int gridX, int gridY) const;

            // Drawing helpers (extracted from render for clarity)
            void drawWallSprite(sf::RenderWindow &window, const Game::Wall &wall, bool isPreview);
            void drawTilesRow(sf::RenderWindow &window, int row);
            void drawPawnsRow(sf::RenderWindow &window, const std::vector<Game::Pawn> &pawns, int row);
            void drawWallsRow(sf::RenderWindow &window, const std::vector<Game::Wall> &walls, int row);

            // Textures (Must stay alive while game runs)
            sf::Texture m_texTile;

            // Flyweight Sprite for tiles
            sf::Sprite m_spriteTile;
            // Wall preview texture/sprite (renderer-owned)
            sf::Texture m_texWall;
            sf::Sprite m_spriteWall;

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
            GameRenderer();

            // Load textures and setup sprites
            bool init();

            // Draw everything
            void render(sf::RenderWindow &window, const Game::Board &board);

            // Handle resizing (centering the isometric board)
            void handleResize(sf::RenderWindow &window, sf::Vector2u size);

            // Updates the hovered tile indices
            void setHoveredTile(sf::Vector2i gridCoords);

            // Converts Screen Pixel (Mouse) -> Grid Index (0..8) / Returns {-1, -1} if outside board
            sf::Vector2i getMouseGridPos(const sf::RenderWindow& window, sf::Vector2i mousePos) const;

            // Wall Preview Logic 
            void setWallPreview(bool active, sf::Vector2i gridPos, Game::Orientation orientation);
    };

}
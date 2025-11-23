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

            // Textures (Must stay alive while game runs)
            sf::Texture m_texTile;
            sf::Texture m_texPawnP1;
            sf::Texture m_texPawnP2;

            // Flyweight Sprites
            sf::Sprite m_spriteTile;
            sf::Sprite m_spritePawn;

            // Isometric Layout Config
            sf::Vector2f m_boardOrigin; // Screen center offset
            float m_isoWidth;           // 112.f
            float m_isoHeight;          // 57.f

            sf::View m_view;

            sf::Vector2i m_hoveredCoords = {-1, -1};

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
    };

}
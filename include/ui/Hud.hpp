#pragma once
#include <SFML/Graphics.hpp>

namespace UI
{

    class Hud
    {
    private:
        sf::Texture m_player1IndicatorTexture;
        sf::Texture m_player2IndicatorTexture;
        sf::Sprite m_player1IndicatorSprite;
        sf::Sprite m_player2IndicatorSprite;
        bool m_hasIndicators = false;
        sf::Font m_font;
        sf::Text m_player1WallsText;
        sf::Text m_player2WallsText;
        bool m_hasFont = false;
        int m_currentPlayerId = 1;

    public:
        Hud(); // Build HUD state #

        bool init(); // Load indicator assets and font #
        void update(int currentPlayerId, int p1WallsRemaining, int p2WallsRemaining, int maxWallsPerPlayer); // Update counters #
        void render(sf::RenderWindow &window); // Draw HUD #
    };
}

#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace UI
{

    class Hud
    {
        private:
            sf::Font m_font;
            sf::Text m_turnText;
            
        public:
            Hud();

            bool init();
            void update(int currentPlayerId);
            void render(sf::RenderWindow &window);
    };
}
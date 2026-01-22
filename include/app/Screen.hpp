#pragma once
#include <SFML/Graphics.hpp>

namespace App
{
    class Screen
    {
    public:
        virtual ~Screen() = default;
        virtual bool init() { return true; }
        virtual void handleEvent(const sf::Event &event, sf::RenderWindow &window) = 0;
        virtual void handleResize(sf::RenderWindow &window, sf::Vector2u size) = 0;
        virtual void update(float dt) = 0;
        virtual void render(sf::RenderWindow &window) = 0;
    };
}

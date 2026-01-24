#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "ui/StateComponent.hpp"

namespace UI
{
    class Button : public StateComponent
    {
    public:
        Button(); // Build button state #
        bool init(const std::string &tilePath,
                  const std::string &hoverTilePath,
                  const std::string &fontPath,
                  const std::string &label,
                  unsigned int fontSize = 25); // Load assets and label #

        void setLabel(const std::string &label); // Update label text #
        void setTextColor(const sf::Color &color); // Update label color #
        void setOnClick(std::function<void()> onClick); // Register click callback #

        void setPosition(const sf::Vector2f &position) override; // Move button #
        void setScale(const sf::Vector2f &scale) override; // Scale button #
        void render(sf::RenderWindow &window) const override; // Draw button #

    protected:
        void onClick() override; // Handle click event #

    private:
        void updateLabelPosition(); // Center label in button #

        sf::Font m_font;
        sf::Text m_label;
        unsigned int m_baseFontSize = 36;
        std::function<void()> m_onClick;
    };
}

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
        Button();
        bool init(const std::string &tilePath,
                  const std::string &hoverTilePath,
                  const std::string &fontPath,
                  const std::string &label,
                  unsigned int fontSize = 25);

        void setLabel(const std::string &label);
        void setTextColor(const sf::Color &color);
        void setOnClick(std::function<void()> onClick);

        void setPosition(const sf::Vector2f &position) override;
        void setScale(const sf::Vector2f &scale) override;
        void render(sf::RenderWindow &window) const override;

    protected:
        void onClick() override;

    private:
        void updateLabelPosition();

        sf::Font m_font;
        sf::Text m_label;
        unsigned int m_baseFontSize = 36;
        std::function<void()> m_onClick;
    };
}

#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "ui/StateComponent.hpp"

namespace UI
{
    class Menu
    {
    public:
        Menu();
        virtual ~Menu() = default;

        bool initBackground(const std::string &path);

        void setEnabled(bool enabled);
        void toggleEnabled();
        bool isEnabled() const;

        void addButton(std::unique_ptr<StateComponent> button);

        void updateHover(const sf::RenderWindow &window, sf::Vector2i mousePos);
        bool handleClick(const sf::RenderWindow &window, sf::Vector2i mousePos);

        virtual void render(sf::RenderWindow &window);

    protected:
        virtual void layout(const sf::RenderWindow &window);
        void setScreenBackdropEnabled(bool enabled);
        void setScreenBackdropColors(const sf::Color &top, const sf::Color &bottom);
        void updateScreenBackdrop(const sf::View &view);

        bool m_enabled = false;
        sf::Texture m_backgroundTexture;
        sf::Sprite m_backgroundSprite;
        bool m_hasBackground = false;
        bool m_screenBackdropEnabled = false;
        sf::VertexArray m_screenBackdrop;
        sf::Color m_screenBackdropTop = sf::Color(0, 0, 0, 120);
        sf::Color m_screenBackdropBottom = sf::Color(0, 0, 0, 200);

        std::vector<std::unique_ptr<StateComponent>> m_buttons;
        float m_buttonGap = 26.f;
    };
}

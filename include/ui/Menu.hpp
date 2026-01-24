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
        Menu(); // Build menu state #
        virtual ~Menu() = default; // Polymorphic cleanup #

        bool initBackground(const std::string &path); // Load background texture #

        void setEnabled(bool enabled); // Enable or disable menu #
        void toggleEnabled(); // Toggle enabled state #
        bool isEnabled() const; // Query enabled state #

        void addButton(std::unique_ptr<StateComponent> button); // Add a button #

        void updateHover(const sf::RenderWindow &window, sf::Vector2i mousePos); // Update hover state #
        bool handleClick(const sf::RenderWindow &window, sf::Vector2i mousePos); // Handle click #

        virtual void render(sf::RenderWindow &window); // Draw menu #

    protected:
        virtual void layout(const sf::RenderWindow &window); // Compute layout #
        void setScreenBackdropEnabled(bool enabled); // Enable backdrop #
        void setScreenBackdropColors(const sf::Color &top, const sf::Color &bottom); // Set backdrop colors #
        void updateScreenBackdrop(const sf::View &view); // Update backdrop geometry #

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

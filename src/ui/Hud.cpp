#include "ui/Hud.hpp"
#include <iostream>

namespace UI
{

    Hud::Hud()
        : m_turnText(m_font)
    {
    }

    bool Hud::init()
    {
        // Load Font
        if (!m_font.openFromFile("assets/fonts/roboto.ttf"))
        {
            std::cerr << "Failed to load font: assets/fonts/roboto.ttf" << std::endl;
            return false;
        }

        m_turnText.setCharacterSize(30);
        m_turnText.setFillColor(sf::Color::White);
        m_turnText.setPosition({20.f, 20.f});
        m_turnText.setString("Turn: Player 1");

        return true;
    }

    void Hud::update(int currentPlayerId)
    {
        std::string str = "Turn: Player " + std::to_string(currentPlayerId);
        m_turnText.setString(str);

        if (currentPlayerId == 1)
            m_turnText.setFillColor(sf::Color(100, 100, 255));
        else
            m_turnText.setFillColor(sf::Color::Red);
    }

    void Hud::render(sf::RenderWindow &window)
    {
        // Save current view (Isometric)
        sf::View oldView = window.getView();

        // Switch to 2D Screen view for UI
        window.setView(window.getDefaultView());

        window.draw(m_turnText);

        // Restore World view
        window.setView(oldView);
    }
}
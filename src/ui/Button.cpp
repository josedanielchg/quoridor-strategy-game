#include "ui/Button.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>

namespace UI
{
    Button::Button()
        : m_label(m_font)
    {
    }

    bool Button::init(const std::string &tilePath,
                      const std::string &hoverTilePath,
                      const std::string &fontPath,
                      const std::string &label,
                      unsigned int fontSize)
    {
        if (!initTiles(tilePath, hoverTilePath))
            return false;

        if (!m_font.openFromFile(fontPath))
        {
            std::cerr << "Failed to load font: " << fontPath << " (cwd: "
                      << std::filesystem::current_path().string() << ")\n";
            return false;
        }

        m_baseFontSize = fontSize;
        m_label.setFont(m_font);
        m_label.setString(label);
        m_label.setFillColor(sf::Color::White);
        m_label.setCharacterSize(m_baseFontSize);

        updateLabelPosition();
        return true;
    }

    void Button::setLabel(const std::string &label)
    {
        m_label.setString(label);
        updateLabelPosition();
    }

    void Button::setTextColor(const sf::Color &color)
    {
        m_label.setFillColor(color);
    }

    void Button::setOnClick(std::function<void()> onClick)
    {
        m_onClick = std::move(onClick);
    }

    void Button::setPosition(const sf::Vector2f &position)
    {
        StateComponent::setPosition(position);
        updateLabelPosition();
    }

    void Button::setScale(const sf::Vector2f &scale)
    {
        StateComponent::setScale(scale);
        const float uniformScale = std::min(scale.x, scale.y);
        const unsigned int size = std::max(1u, static_cast<unsigned int>(std::round(m_baseFontSize * uniformScale)));
        m_label.setCharacterSize(size);
        updateLabelPosition();
    }

    void Button::render(sf::RenderWindow &window) const
    {
        StateComponent::render(window);
        window.draw(m_label);
    }

    void Button::onClick()
    {
        if (m_onClick)
            m_onClick();
    }

    void Button::updateLabelPosition()
    {
        const sf::FloatRect bounds = m_label.getLocalBounds();
        m_label.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                           bounds.position.y + bounds.size.y / 2.f});
        m_label.setPosition(position());
    }
}

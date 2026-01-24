#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace UI
{
    enum class ComponentState
    {
        Normal,
        Hovered
    };

    class StateComponent
    {
    public:
        StateComponent(); // Build component state #
        virtual ~StateComponent() = default; // Polymorphic cleanup #

        bool initTiles(const std::string &tilePath, const std::string &hoverTilePath = std::string()); // Load tile textures #

        virtual void setPosition(const sf::Vector2f &position); // Set component position #
        virtual void setScale(const sf::Vector2f &scale); // Set component scale #

        sf::Vector2f position() const; // Get position #
        sf::Vector2f size() const; // Get size #
        sf::FloatRect bounds() const; // Get bounds #

        void updateHover(const sf::Vector2f &mousePos); // Update hover state #
        bool handleClick(const sf::Vector2f &mousePos); // Handle click #

        virtual void render(sf::RenderWindow &window) const; // Draw component #

    protected:
        virtual void onClick(); // Hook for click actions #

        ComponentState m_state = ComponentState::Normal;
        sf::Texture m_tileTexture;
        sf::Texture m_hoverTileTexture;
        sf::Sprite m_tileSprite;
        sf::Sprite m_hoverTileSprite;

    private:
        void setCenteredOrigin(sf::Sprite &sprite, const sf::Texture &texture); // Center sprite origin #
    };
}

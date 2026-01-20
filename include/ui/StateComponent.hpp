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
        StateComponent();
        virtual ~StateComponent() = default;

        bool initTiles(const std::string &tilePath, const std::string &hoverTilePath = std::string());

        virtual void setPosition(const sf::Vector2f &position);
        virtual void setScale(const sf::Vector2f &scale);

        sf::Vector2f position() const;
        sf::Vector2f size() const;
        sf::FloatRect bounds() const;

        void updateHover(const sf::Vector2f &mousePos);
        bool handleClick(const sf::Vector2f &mousePos);

        virtual void render(sf::RenderWindow &window) const;

    protected:
        virtual void onClick();

        ComponentState m_state = ComponentState::Normal;
        sf::Texture m_tileTexture;
        sf::Texture m_hoverTileTexture;
        sf::Sprite m_tileSprite;
        sf::Sprite m_hoverTileSprite;

    private:
        void setCenteredOrigin(sf::Sprite &sprite, const sf::Texture &texture);
    };
}

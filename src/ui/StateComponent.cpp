#include "ui/StateComponent.hpp"
#include "audio/SfxManager.hpp"

namespace UI
{
    StateComponent::StateComponent()
        : m_tileSprite(m_tileTexture),
          m_hoverTileSprite(m_hoverTileTexture)
    {
    }

    bool StateComponent::initTiles(const std::string &tilePath, const std::string &hoverTilePath)
    {
        if (!m_tileTexture.loadFromFile(tilePath))
            return false;

        const std::string hoverPath = hoverTilePath.empty() ? tilePath : hoverTilePath;
        if (!m_hoverTileTexture.loadFromFile(hoverPath))
            return false;

        m_tileSprite.setTexture(m_tileTexture, true);
        m_hoverTileSprite.setTexture(m_hoverTileTexture, true);

        setCenteredOrigin(m_tileSprite, m_tileTexture);
        setCenteredOrigin(m_hoverTileSprite, m_hoverTileTexture);

        if (hoverTilePath.empty())
            m_hoverTileSprite.setColor(sf::Color(255, 255, 255, 210));

        return true;
    }

    void StateComponent::setPosition(const sf::Vector2f &position)
    {
        m_tileSprite.setPosition(position);
        m_hoverTileSprite.setPosition(position);
    }

    void StateComponent::setScale(const sf::Vector2f &scale)
    {
        m_tileSprite.setScale(scale);
        m_hoverTileSprite.setScale(scale);
    }

    sf::Vector2f StateComponent::position() const
    {
        return m_tileSprite.getPosition();
    }

    sf::Vector2f StateComponent::size() const
    {
        const sf::FloatRect rect = m_tileSprite.getGlobalBounds();
        return rect.size;
    }

    sf::FloatRect StateComponent::bounds() const
    {
        return m_tileSprite.getGlobalBounds();
    }

    void StateComponent::updateHover(const sf::Vector2f &mousePos)
    {
        const bool isHovered = bounds().contains(mousePos);
        const ComponentState nextState = isHovered ? ComponentState::Hovered : ComponentState::Normal;
        if (nextState != m_state && nextState == ComponentState::Hovered)
            Audio::SfxManager::instance().play(Audio::SfxId::Hover);
        m_state = nextState;
    }

    bool StateComponent::handleClick(const sf::Vector2f &mousePos)
    {
        if (!bounds().contains(mousePos))
            return false;

        Audio::SfxManager::instance().play(Audio::SfxId::Click);
        onClick();
        return true;
    }

    void StateComponent::render(sf::RenderWindow &window) const
    {
        if (m_state == ComponentState::Hovered)
            window.draw(m_hoverTileSprite);
        else
            window.draw(m_tileSprite);
    }

    void StateComponent::onClick()
    {
    }

    void StateComponent::setCenteredOrigin(sf::Sprite &sprite, const sf::Texture &texture)
    {
        const sf::Vector2u size = texture.getSize();
        sprite.setOrigin({static_cast<float>(size.x) / 2.f,
                          static_cast<float>(size.y) / 2.f});
    }
}

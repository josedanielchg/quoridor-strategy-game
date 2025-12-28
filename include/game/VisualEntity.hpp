#pragma once
#include "game/Entity.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace Game
{

    // VisualEntity: Adds simple SFML Texture/Sprite handling to Entities
    class VisualEntity : public Entity
    {
    protected:
        sf::Texture m_texture;
        sf::Sprite m_sprite;

    public:
        VisualEntity(int x, int y) : Entity(x, y) {}
        virtual ~VisualEntity() = default;

        // Load texture from path returned by getTexturePath(), setup sprite and origin
        bool initSprite()
        {
            const std::string path = getTexturePath();
            if (path.empty())
                return true; // Nothing to load

            if (!m_texture.loadFromFile(path))
            {
                std::cerr << "Error: Could not load " << path << "\n";
                return false;
            }

            m_sprite.setTexture(m_texture, true);

            sf::Vector2u texSize = m_texture.getSize();
            sf::Vector2f origin = getSpriteOrigin(texSize);
            m_sprite.setOrigin(origin);

            return true;
        }

        // Child classes may override to provide custom origin
        virtual sf::Vector2f getSpriteOrigin(const sf::Vector2u &texSize) const
        {
            return {float(texSize.x) / 2.f, float(texSize.y) / 2.f};
        }

        // Access the sprite for rendering (copy if you need to mutate safely)
        const sf::Sprite &sprite() const { return m_sprite; }
        sf::Sprite &sprite() { return m_sprite; }
    };

}

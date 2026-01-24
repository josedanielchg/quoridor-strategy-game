#pragma once
#include "game/Entity.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <utility>

namespace Game
{

    class VisualEntity : public Entity
    {
    protected:
        sf::Texture m_texture;
        std::unique_ptr<sf::Sprite> m_sprite;

    public:
        VisualEntity(int x, int y) : Entity(x, y), m_sprite(nullptr) {} // Init visual entity #
        virtual ~VisualEntity() = default; // Polymorphic cleanup #
        VisualEntity(const VisualEntity &) = delete; // No copy #
        VisualEntity &operator=(const VisualEntity &) = delete; // No copy #
        VisualEntity(VisualEntity &&other) noexcept // Move construct #
            : Entity(std::move(other)), m_texture(std::move(other.m_texture)), m_sprite(std::move(other.m_sprite))
        {
            if (m_sprite)
                m_sprite->setTexture(m_texture, false);
        }
        VisualEntity &operator=(VisualEntity &&other) noexcept // Move assign #
        {
            if (this != &other)
            {
                Entity::operator=(std::move(other));
                m_texture = std::move(other.m_texture);
                m_sprite = std::move(other.m_sprite);
                if (m_sprite)
                    m_sprite->setTexture(m_texture, false);
            }
            return *this;
        }

        bool initSprite() // Load texture and build sprite #
        {
            const std::string path = getTexturePath();
            if (path.empty())
                return true;

            if (!m_texture.loadFromFile(path))
            {
                std::cerr << "Error: Could not load " << path << "\n";
                return false;
            }

            m_sprite = std::make_unique<sf::Sprite>(m_texture);

            sf::Vector2u texSize = m_texture.getSize();
            sf::Vector2f origin = getSpriteOrigin(texSize);
            m_sprite->setOrigin(origin);

            return true;
        }

        virtual sf::Vector2f getSpriteOrigin(const sf::Vector2u &texSize) const // Default sprite origin #
        {
            return {float(texSize.x) / 2.f, float(texSize.y) / 2.f};
        }

        const sf::Sprite &sprite() const { return *m_sprite; } // Read-only sprite #
        sf::Sprite &sprite() { return *m_sprite; } // Mutable sprite #
    };

}

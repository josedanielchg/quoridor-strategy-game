#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ui/UiConstants.hpp"

namespace UI
{
    // Base class that owns a texture and a sprite and provides
    // a standardized initialization path for loading and setting origin.
    class SpriteEntity
    {
    protected:
        sf::Texture m_texture;
        sf::Sprite m_sprite;

    public:
        SpriteEntity() = default;
        virtual ~SpriteEntity() = default;

        // Load texture from file and setup sprite (texture + origin)
        bool initFromFile(const std::string &path)
        {
            if (!m_texture.loadFromFile(path))
                return false;

            m_sprite.setTexture(m_texture, true);
            m_sprite.setOrigin(computeOrigin(m_texture));
            return true;
        }

        // Subclasses compute the proper origin for their sprite
        virtual sf::Vector2f computeOrigin(const sf::Texture &tex) const = 0;

        // Access the underlying sprite for drawing or modification
        sf::Sprite &sprite() { return m_sprite; }
        const sf::Sprite &sprite() const { return m_sprite; }
    };

    // Tile: origin at texture center
    class TileSprite : public SpriteEntity
    {
    public:
        sf::Vector2f computeOrigin(const sf::Texture &tex) const override
        {
            sf::Vector2u size = tex.getSize();
            return {float(size.x) / 2.f, float(size.y) / 2.f};
        }
    };

    // Wall: origin at texture center
    class WallSprite : public SpriteEntity
    {
    public:
        sf::Vector2f computeOrigin(const sf::Texture &tex) const override
        {
            sf::Vector2u size = tex.getSize();
            return {float(size.x) / 2.f, float(size.y) / 2.f};
        }
    };

    // Pawn: special origin (uses constants rather than texture center)
    class PawnSprite : public SpriteEntity
    {
    private:
        sf::Texture m_textureP2; // second player texture

    public:
        // Load both pawn textures (primary path for player 1, alt for player 2)
        bool initFromFiles(const std::string &pathP1, const std::string &pathP2)
        {
            if (!m_texture.loadFromFile(pathP1))
                return false;
            if (!m_textureP2.loadFromFile(pathP2))
                return false;

            m_sprite.setTexture(m_texture, true);
            m_sprite.setOrigin(computeOrigin(m_texture));
            return true;
        }

        // Choose which texture to use for the sprite (1 or 2)
        void setPlayerTexture(int playerId)
        {
            if (playerId == 1)
                m_sprite.setTexture(m_texture, true);
            else
                m_sprite.setTexture(m_textureP2, true);
        }

        // Pawn uses a fixed origin (matching previous logic)
        sf::Vector2f computeOrigin(const sf::Texture &tex) const override
        {
            sf::Vector2u size = tex.getSize();
            return {float(size.x) / 2.f, float(size.y) - UI::PAWN_BASELINE_OFFSET};
        }
    };
}

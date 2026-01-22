#include "ui/Hud.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

namespace UI
{
    static const sf::Vector2f UI_CANVAS_SIZE = {1728.f, 1117.f};
    static const sf::Vector2f INDICATOR_DESIGN_SIZE = {397.f, 128.f};
    static constexpr float INDICATOR_MARGIN = 60.f;
    static constexpr float WALLS_TEXT_RIGHT_PADDING = 39.f;
    static constexpr float WALLS_TEXT_FONT_SIZE = 45.f;

    Hud::Hud()
        : m_player1IndicatorSprite(m_player1IndicatorTexture),
          m_player2IndicatorSprite(m_player2IndicatorTexture),
          m_player1WallsText(m_font),
          m_player2WallsText(m_font)
    {
    }

    bool Hud::init()
    {
        if (!m_player1IndicatorTexture.loadFromFile("assets/textures/player1-indicator.png"))
        {
            std::cerr << "Failed to load player1-indicator.png" << std::endl;
            return false;
        }

        if (!m_player2IndicatorTexture.loadFromFile("assets/textures/player2-indicator.png"))
        {
            std::cerr << "Failed to load player2-indicator.png" << std::endl;
            return false;
        }

        m_player1IndicatorSprite.setTexture(m_player1IndicatorTexture, true);
        m_player2IndicatorSprite.setTexture(m_player2IndicatorTexture, true);
        m_player2IndicatorSprite.setOrigin({0.f, 0.f});

        m_hasIndicators = true;

        if (!m_font.openFromFile("assets/fonts/roboto.ttf"))
        {
            std::cerr << "Failed to load font: assets/fonts/roboto.ttf" << std::endl;
            return false;
        }

        m_player1WallsText.setFillColor(sf::Color::White);
        m_player2WallsText.setFillColor(sf::Color::White);
        m_hasFont = true;

        return true;
    }

    void Hud::update(int currentPlayerId, int p1WallsRemaining, int p2WallsRemaining, int maxWallsPerPlayer)
    {
        (void)currentPlayerId;
        m_player1WallsText.setString(std::to_string(p1WallsRemaining) + "/" + std::to_string(maxWallsPerPlayer));
        m_player2WallsText.setString(std::to_string(p2WallsRemaining) + "/" + std::to_string(maxWallsPerPlayer));
    }

    void Hud::render(sf::RenderWindow &window)
    {
        // Save current view (Isometric)
        sf::View oldView = window.getView();

        // Switch to 2D Screen view for UI
        sf::View uiView = UI::makeLetterboxView(window.getSize());
        window.setView(uiView);

        if (m_hasIndicators)
        {
            const sf::Vector2f viewSize = uiView.getSize();
            const sf::Vector2f viewCenter = uiView.getCenter();
            const sf::Vector2f topLeft = {viewCenter.x - viewSize.x / 2.f,
                                          viewCenter.y - viewSize.y / 2.f};

            const float scale = std::max(viewSize.x / UI_CANVAS_SIZE.x,
                                         viewSize.y / UI_CANVAS_SIZE.y);
            const sf::Vector2f scaledIndicatorSize = {INDICATOR_DESIGN_SIZE.x * scale,
                                                      INDICATOR_DESIGN_SIZE.y * scale};
            const sf::Vector2f margin = {INDICATOR_MARGIN * scale,
                                         INDICATOR_MARGIN * scale};

            const sf::Vector2u p1TexSize = m_player1IndicatorTexture.getSize();
            const sf::Vector2u p2TexSize = m_player2IndicatorTexture.getSize();

            m_player1IndicatorSprite.setScale({scaledIndicatorSize.x / float(p1TexSize.x),
                                               scaledIndicatorSize.y / float(p1TexSize.y)});
            m_player2IndicatorSprite.setScale({scaledIndicatorSize.x / float(p2TexSize.x),
                                               scaledIndicatorSize.y / float(p2TexSize.y)});

            m_player1IndicatorSprite.setPosition({topLeft.x + margin.x,
                                                  topLeft.y + margin.y});
            m_player2IndicatorSprite.setPosition({topLeft.x + viewSize.x - margin.x - scaledIndicatorSize.x,
                                                  topLeft.y + margin.y});

            window.draw(m_player1IndicatorSprite);
            window.draw(m_player2IndicatorSprite);

            if (m_hasFont)
            {
                const unsigned int fontSize = std::max(1u, static_cast<unsigned int>(
                                                              std::round(WALLS_TEXT_FONT_SIZE * scale)));
                m_player1WallsText.setCharacterSize(fontSize);
                m_player2WallsText.setCharacterSize(fontSize);

                const float textRightPadding = WALLS_TEXT_RIGHT_PADDING * scale;
                const float textCenterY = topLeft.y + margin.y + scaledIndicatorSize.y / 2.f;

                const sf::Vector2f p1TopLeft = m_player1IndicatorSprite.getPosition();
                const sf::Vector2f p2TopLeft = m_player2IndicatorSprite.getPosition();

                auto positionRightAlignedText = [](sf::Text &text, float x, float y)
                {
                    const sf::FloatRect bounds = text.getLocalBounds();
                    text.setOrigin({bounds.position.x + bounds.size.x,
                                    bounds.position.y + bounds.size.y / 2.f});
                    text.setPosition({x, y});
                };
                auto positionLeftAlignedText = [](sf::Text &text, float x, float y)
                {
                    const sf::FloatRect bounds = text.getLocalBounds();
                    text.setOrigin({bounds.position.x,
                                    bounds.position.y + bounds.size.y / 2.f});
                    text.setPosition({x, y});
                };

                positionRightAlignedText(m_player1WallsText,
                                         p1TopLeft.x + scaledIndicatorSize.x - textRightPadding,
                                         textCenterY);
                positionLeftAlignedText(m_player2WallsText,
                                        p2TopLeft.x + textRightPadding,
                                        textCenterY);

                window.draw(m_player1WallsText);
                window.draw(m_player2WallsText);
            }
        }

        // Restore World view
        window.setView(oldView);
    }
}

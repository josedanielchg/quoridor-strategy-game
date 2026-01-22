#include "app/TitleScreen.hpp"
#include "ui/UiConstants.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <utility>

namespace App
{
    static constexpr unsigned int PROMPT_FONT_SIZE = 38;

    TitleScreen::TitleScreen()
        : m_backgroundSprite(m_backgroundTexture),
          m_promptText(m_font)
    {
    }

    bool TitleScreen::init()
    {
        if (!m_backgroundTexture.loadFromFile("assets/textures/quoridor-bg-sprite-sheet.png"))
        {
            std::cerr << "Failed to load title background\n";
            return false;
        }

        m_backgroundSprite.setTexture(m_backgroundTexture, true);
        m_backgroundSprite.setTextureRect(
            sf::IntRect({0, 0}, {FRAME_WIDTH, FRAME_HEIGHT}));
        m_backgroundSprite.setOrigin({FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f});

        if (!m_font.openFromFile("assets/fonts/roboto.ttf"))
        {
            std::cerr << "Failed to load font for title screen\n";
            return false;
        }

        m_promptText.setFont(m_font);
        m_promptText.setString("PLEASE PRESS ANY KEY TO START");
        m_promptText.setFillColor(sf::Color::White);
        m_promptText.setCharacterSize(PROMPT_FONT_SIZE);

        return true;
    }

    void TitleScreen::setOnStart(std::function<void()> onStart)
    {
        m_onStart = std::move(onStart);
    }

    void TitleScreen::handleEvent(const sf::Event &event, sf::RenderWindow & /*window*/)
    {
        if (event.is<sf::Event::KeyPressed>())
        {
            if (m_onStart)
                m_onStart();
        }
    }

    void TitleScreen::handleResize(sf::RenderWindow &window, sf::Vector2u /*size*/)
    {
        updateLayout(UI::makeLetterboxView(window.getSize()));
    }

    void TitleScreen::update(float dt)
    {
        m_blinkTimer += dt;
        const float pulse = (std::sin(m_blinkTimer * 3.5f) + 1.f) * 0.5f;
        const std::uint8_t alpha = static_cast<std::uint8_t>(120 + pulse * 135);
        sf::Color color = m_promptText.getFillColor();
        color.a = alpha;
        m_promptText.setFillColor(color);

        m_frameTimer += dt;
        const int fullRows = std::max(0, SPRITE_ROWS - 1);
        const int totalFrames = fullRows * SPRITE_COLUMNS + LAST_ROW_FRAMES;
        while (m_frameTimer >= FRAME_TIME && totalFrames > 0)
        {
            m_frameTimer -= FRAME_TIME;
            m_frameIndex = (m_frameIndex + 1) % totalFrames;
            const int col = m_frameIndex % SPRITE_COLUMNS;
            const int row = m_frameIndex / SPRITE_COLUMNS;
            m_backgroundSprite.setTextureRect(
                sf::IntRect({col * FRAME_WIDTH, row * FRAME_HEIGHT},
                            {FRAME_WIDTH, FRAME_HEIGHT}));
        }
    }

    void TitleScreen::render(sf::RenderWindow &window)
    {
        sf::View oldView = window.getView();
        sf::View uiView = UI::makeLetterboxView(window.getSize());
        window.setView(uiView);

        window.draw(m_backgroundSprite);
        window.draw(m_promptText);

        window.setView(oldView);
    }

    void TitleScreen::updateLayout(const sf::View &view)
    {
        const sf::Vector2f viewSize = view.getSize();
        const sf::Vector2f viewCenter = view.getCenter();

        const float scaleX = viewSize.x / static_cast<float>(FRAME_WIDTH);
        const float scaleY = viewSize.y / static_cast<float>(FRAME_HEIGHT);
        const float scale = std::max(scaleX, scaleY);
        m_backgroundSprite.setScale({scale, scale});
        m_backgroundSprite.setPosition(viewCenter);

        const float textScale = std::min(viewSize.x / UI::UI_DESIGN_SIZE.x,
                                         viewSize.y / UI::UI_DESIGN_SIZE.y);
        const unsigned int textSize = std::max(1u, static_cast<unsigned int>(std::round(PROMPT_FONT_SIZE * textScale)));
        m_promptText.setCharacterSize(textSize);
        const sf::FloatRect bounds = m_promptText.getLocalBounds();
        m_promptText.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                                bounds.position.y + bounds.size.y / 2.f});

        m_promptText.setPosition({viewCenter.x, viewCenter.y + viewSize.y * 0.28f});
    }
}

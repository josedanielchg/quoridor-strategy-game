#include "app/TitleScreen.hpp"
#include "resources/ResourceLoader.hpp"
#include "ui/UiConstants.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <filesystem>
#include <utility>

namespace App
{
    static constexpr unsigned int PROMPT_FONT_SIZE = 38;
    static const sf::Color PROMPT_ACCENT_COLOR(0xFD, 0x41, 0x04);

    TitleScreen::TitleScreen()
        : m_backgroundSprite(m_backgroundTexture),
          m_promptLine1(m_font),
          m_promptLine2(m_font),
          m_promptGradient(sf::PrimitiveType::TriangleStrip, 8)
    {
    }

    bool TitleScreen::init()
    {
        Resources::loadTextureInto(m_backgroundTexture,
                                   "assets/textures/quoridor-bg-sprite-sheet.png",
                                   "TitleScreen",
                                   "Background spritesheet");

        m_backgroundSprite.setTexture(m_backgroundTexture, true);
        m_backgroundSprite.setTextureRect(
            sf::IntRect({0, 0}, {FRAME_WIDTH, FRAME_HEIGHT}));
        m_backgroundSprite.setOrigin({FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f});

        Resources::loadFontInto(m_font,
                                "assets/fonts/pixelon.ttf",
                                "TitleScreen",
                                "UI font");

        m_promptLine1.setFont(m_font);
        m_promptLine1.setString("PLEASE PRESS ANY KEY");
        m_promptLine1.setFillColor(sf::Color::White);
        m_promptLine1.setCharacterSize(PROMPT_FONT_SIZE);

        m_promptLine2.setFont(m_font);
        m_promptLine2.setString("TO START");
        m_promptLine2.setFillColor(PROMPT_ACCENT_COLOR);
        m_promptLine2.setCharacterSize(PROMPT_FONT_SIZE);

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
        sf::Color color1 = m_promptLine1.getFillColor();
        color1.a = alpha;
        m_promptLine1.setFillColor(color1);

        sf::Color color2 = m_promptLine2.getFillColor();
        color2.a = alpha;
        m_promptLine2.setFillColor(color2);

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
        window.draw(m_promptGradient);
        window.draw(m_promptLine1);
        window.draw(m_promptLine2);

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
        m_promptLine1.setCharacterSize(textSize);
        m_promptLine2.setCharacterSize(textSize);

        const sf::FloatRect bounds1 = m_promptLine1.getLocalBounds();
        m_promptLine1.setOrigin({bounds1.position.x + bounds1.size.x / 2.f,
                                 bounds1.position.y + bounds1.size.y / 2.f});

        const sf::FloatRect bounds2 = m_promptLine2.getLocalBounds();
        m_promptLine2.setOrigin({bounds2.position.x + bounds2.size.x / 2.f,
                                 bounds2.position.y + bounds2.size.y / 2.f});

        const sf::Vector2f containerSize{672.f * textScale, 125.f * textScale};
        const sf::Vector2f containerCenter{viewCenter.x, viewCenter.y + viewSize.y * 0.12f};

        const float lineSpacing = m_font.getLineSpacing(textSize);
        const float totalHeight = lineSpacing * 2.f;
        const float line1Y = containerCenter.y - totalHeight / 2.f + lineSpacing / 2.f;
        const float line2Y = line1Y + lineSpacing;
        m_promptLine1.setPosition({containerCenter.x, line1Y});
        m_promptLine2.setPosition({containerCenter.x, line2Y});

        const float left = containerCenter.x - containerSize.x / 2.f;
        const float right = containerCenter.x + containerSize.x / 2.f;
        const float top = containerCenter.y - containerSize.y / 2.f;
        const float bottom = containerCenter.y + containerSize.y / 2.f;
        const float x0 = left;
        const float x1 = left + containerSize.x * 0.25f;
        const float x2 = left + containerSize.x * 0.75f;
        const float x3 = right;

        const sf::Color transparent(0, 0, 0, 0);
        const sf::Color opaque(0, 0, 0, 255);

        m_promptGradient[0].position = {x0, top};
        m_promptGradient[1].position = {x0, bottom};
        m_promptGradient[2].position = {x1, top};
        m_promptGradient[3].position = {x1, bottom};
        m_promptGradient[4].position = {x2, top};
        m_promptGradient[5].position = {x2, bottom};
        m_promptGradient[6].position = {x3, top};
        m_promptGradient[7].position = {x3, bottom};

        m_promptGradient[0].color = transparent;
        m_promptGradient[1].color = transparent;
        m_promptGradient[2].color = opaque;
        m_promptGradient[3].color = opaque;
        m_promptGradient[4].color = opaque;
        m_promptGradient[5].color = opaque;
        m_promptGradient[6].color = transparent;
        m_promptGradient[7].color = transparent;
    }
}

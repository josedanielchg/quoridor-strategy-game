#include "app/CreditsScreen.hpp"
#include "ui/UiConstants.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace App
{
    static constexpr unsigned int TITLE_FONT_SIZE = 36;
    static constexpr unsigned int BODY_FONT_SIZE = 26;
    static const sf::Color SHADOW_COLOR(0, 0, 0, 180);
    static const char *BACKGROUND_PATH = "assets/textures/credits-bg-spirte-sheet.png";
    static const char *FONT_PATH = "assets/fonts/pixelon.ttf";

    CreditsScreen::CreditsScreen()
        : m_backgroundSprite(m_backgroundTexture),
          m_titleText(m_font),
          m_bodyText(m_font),
          m_titleShadow(m_font),
          m_bodyShadow(m_font)
    {
    }

    bool CreditsScreen::init()
    {
        if (!m_backgroundTexture.loadFromFile(BACKGROUND_PATH))
        {
            std::cerr << "Failed to load credits background\n";
            return false;
        }

        m_backgroundSprite.setTexture(m_backgroundTexture, true);
        m_backgroundSprite.setTextureRect(
            sf::IntRect({0, 0}, {FRAME_WIDTH, FRAME_HEIGHT}));
        m_backgroundSprite.setOrigin({FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f});

        if (!m_font.openFromFile(FONT_PATH))
        {
            std::cerr << "Failed to load font for credits screen\n";
            return false;
        }

        m_titleText.setFillColor(sf::Color::White);
        m_bodyText.setString(
            "CHACON GOMEZ Jose Daniel\n"
            "[ GitHub: https://github.com/josedanielchg ]\n\n"
            "MENESES GAMBOA Carlos Adrian\n"
            "[ GitHub: https://github.com/MenesesCarlos29 ]\n\n"
            "MUSIC\n"
            "Suno (v4.5-all)\n"
            "Tracks used:\n"
            "- Castle Circuits (Version 1)\n"
            "- Castle Circuits (Version 2)\n"
            "[ Source: https://suno.com/ ]\n"
            "[ No commercial use ]\n\n\n"
            "ART / IMAGES\n"
            "Gemini (Nano Banana Pro)\n"
            "[ Source: https://gemini.google.com/ ]\n"
            "[ No commercial use ]"
        );

        m_bodyText.setFillColor(sf::Color::White);

        m_titleShadow.setString(m_titleText.getString());
        m_titleShadow.setFillColor(SHADOW_COLOR);
        m_bodyShadow.setString(m_bodyText.getString());
        m_bodyShadow.setFillColor(SHADOW_COLOR);

        return true;
    }

    void CreditsScreen::setOnBack(std::function<void()> onBack)
    {
        m_onBack = std::move(onBack);
    }

    void CreditsScreen::handleEvent(const sf::Event &event, sf::RenderWindow &window)
    {
        if (event.is<sf::Event::Resized>())
            return;

        if (const auto *key = event.getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scancode::Escape)
            {
                if (m_onBack)
                    m_onBack();
            }
            return;
        }

        if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseBtn->button == sf::Mouse::Button::Left)
            {
                if (m_onBack)
                    m_onBack();
            }
        }
    }

    void CreditsScreen::handleResize(sf::RenderWindow &window, sf::Vector2u /*size*/)
    {
        updateLayout(UI::makeLetterboxView(window.getSize()));
    }

    void CreditsScreen::update(float dt)
    {
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

    void CreditsScreen::render(sf::RenderWindow &window)
    {
        sf::View oldView = window.getView();
        sf::View uiView = UI::makeLetterboxView(window.getSize());
        window.setView(uiView);

        window.draw(m_backgroundSprite);
        window.draw(m_titleShadow);
        window.draw(m_bodyShadow);
        window.draw(m_titleText);
        window.draw(m_bodyText);

        window.setView(oldView);
    }

    void CreditsScreen::updateLayout(const sf::View &view)
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
        m_titleText.setCharacterSize(std::max(1u, static_cast<unsigned int>(std::round(TITLE_FONT_SIZE * textScale))));
        m_bodyText.setCharacterSize(std::max(1u, static_cast<unsigned int>(std::round(BODY_FONT_SIZE * textScale))));
        m_titleShadow.setCharacterSize(m_titleText.getCharacterSize());
        m_bodyShadow.setCharacterSize(m_bodyText.getCharacterSize());

        layoutText(viewCenter, textScale);
    }

    void CreditsScreen::layoutText(const sf::Vector2f &center, float textScale)
    {
        updateTextOrigins(m_titleText);
        updateTextOrigins(m_bodyText);
        updateTextOrigins(m_titleShadow);
        updateTextOrigins(m_bodyShadow);

        const float titleHeight = m_titleText.getLocalBounds().size.y;
        const float bodyHeight = m_bodyText.getLocalBounds().size.y;
        const float gap = 18.f * textScale;
        const float totalHeight = titleHeight + gap + bodyHeight;
        const float topOffset = UI::UI_DESIGN_SIZE.y * 0.045f * textScale;
        const float top = center.y - totalHeight / 2.f + topOffset;

        const float leftOffset = UI::UI_DESIGN_SIZE.x * 0.04f * textScale;
        const sf::Vector2f titlePos(center.x - leftOffset, top + titleHeight / 2.f);
        const sf::Vector2f bodyPos(center.x - leftOffset, top + titleHeight + gap + bodyHeight / 2.f);
        const sf::Vector2f shadowOffset(2.f * textScale, 2.f * textScale);

        m_titleText.setPosition(titlePos);
        m_bodyText.setPosition(bodyPos);
        m_titleShadow.setPosition(titlePos + shadowOffset);
        m_bodyShadow.setPosition(bodyPos + shadowOffset);
    }

    void CreditsScreen::updateTextOrigins(sf::Text &text)
    {
        const sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                        bounds.position.y + bounds.size.y / 2.f});
    }
}

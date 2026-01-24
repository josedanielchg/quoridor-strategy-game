#include "app/MenuScreen.hpp"
#include "audio/SfxManager.hpp"
#include "ui/UiConstants.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <iostream>

namespace App
{
    static constexpr unsigned int MENU_FONT_SIZE = 34;
    static const sf::Color MENU_ACCENT_COLOR(0xFD, 0x41, 0x04);

    // Build menu sprites and gradient geometry. #
    MenuScreen::MenuScreen()
        : m_backgroundSprite(m_backgroundTexture),
          m_menuGradient(sf::PrimitiveType::TriangleStrip, 8)
    {
    }

    // Load background, font, and menu options. #
    bool MenuScreen::init()
    {
        if (!m_backgroundTexture.loadFromFile("assets/textures/quoridor-bg-sprite-sheet.png"))
        {
            std::cerr << "Failed to load menu background\n";
            return false;
        }

        m_backgroundSprite.setTexture(m_backgroundTexture, true);
        m_backgroundSprite.setTextureRect(
            sf::IntRect({0, 0}, {FRAME_WIDTH, FRAME_HEIGHT}));
        m_backgroundSprite.setOrigin({FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f});

        if (!m_font.openFromFile("assets/fonts/pixelon.ttf"))
        {
            std::cerr << "Failed to load font for menu screen (cwd: "
                      << std::filesystem::current_path().string() << ")\n";
            return false;
        }

        m_options = {Option::SinglePlayer,
                     Option::Multiplayer,
                     Option::HowToPlay,
                     Option::Credits};

        const char *labels[] = {"SINGLE PLAYER",
                                "MULTIPLAYER",
                                "HOW TO PLAY",
                                "CREDITS"};

        m_optionTexts.clear();
        m_optionTexts.reserve(m_options.size());
        for (const char *label : labels)
        {
            sf::Text text(m_font);
            text.setString(label);
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(MENU_FONT_SIZE);
            m_optionTexts.push_back(text);
        }

        setSelectedIndex(0);
        return true;
    }

    // Register callback for menu selection. #
    void MenuScreen::setOnOptionSelected(std::function<void(Option)> onSelect)
    {
        m_onOptionSelected = std::move(onSelect);
    }

    // Handle keyboard/mouse input for menu navigation. #
    void MenuScreen::handleEvent(const sf::Event &event, sf::RenderWindow &window)
    {
        if (event.is<sf::Event::Resized>())
            return;

        if (const auto *key = event.getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scancode::Up)
                selectNext(-1);
            else if (key->scancode == sf::Keyboard::Scancode::Down)
                selectNext(1);
            else if (key->scancode == sf::Keyboard::Scancode::Enter ||
                     key->scancode == sf::Keyboard::Scancode::Space)
            {
                Audio::SfxManager::instance().play(Audio::SfxId::Click);
                activateSelected();
            }
            return;
        }

        if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>())
        {
            const sf::View view = UI::makeLetterboxView(window.getSize());
            const sf::Vector2f worldPos = window.mapPixelToCoords(mouseMove->position, view);
            std::size_t newHovered = static_cast<std::size_t>(-1);
            for (std::size_t i = 0; i < m_optionTexts.size(); ++i)
            {
                if (m_optionTexts[i].getGlobalBounds().contains(worldPos))
                {
                    newHovered = i;
                    break;
                }
            }

            if (newHovered != m_hoveredIndex)
            {
                if (newHovered < m_optionTexts.size())
                    Audio::SfxManager::instance().play(Audio::SfxId::Hover);
                m_hoveredIndex = newHovered;
            }
            return;
        }

        if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseBtn->button != sf::Mouse::Button::Left)
                return;

            const sf::View view = UI::makeLetterboxView(window.getSize());
            const sf::Vector2f worldPos = window.mapPixelToCoords(mouseBtn->position, view);
            for (std::size_t i = 0; i < m_optionTexts.size(); ++i)
            {
                if (m_optionTexts[i].getGlobalBounds().contains(worldPos))
                {
                    Audio::SfxManager::instance().play(Audio::SfxId::Click);
                    setSelectedIndex(i);
                    activateSelected();
                    break;
                }
            }
        }
    }

    // Reflow layout on resize. #
    void MenuScreen::handleResize(sf::RenderWindow &window, sf::Vector2u /*size*/)
    {
        updateLayout(UI::makeLetterboxView(window.getSize()));
    }

    // Animate selection blink and background frames. #
    void MenuScreen::update(float dt)
    {
        m_blinkTimer += dt;
        const float pulse = (std::sin(m_blinkTimer * 3.5f) + 1.f) * 0.5f;
        const std::uint8_t alpha = static_cast<std::uint8_t>(120 + pulse * 135);
        applyOptionColors(alpha);

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

    // Draw background, gradient, and menu options. #
    void MenuScreen::render(sf::RenderWindow &window)
    {
        sf::View oldView = window.getView();
        sf::View uiView = UI::makeLetterboxView(window.getSize());
        window.setView(uiView);

        window.draw(m_backgroundSprite);
        window.draw(m_menuGradient);
        for (const auto &text : m_optionTexts)
            window.draw(text);

        window.setView(oldView);
    }

    // Compute positions, sizes, and gradient for current view. #
    void MenuScreen::updateLayout(const sf::View &view)
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
        const unsigned int textSize = std::max(1u, static_cast<unsigned int>(std::round(MENU_FONT_SIZE * textScale)));

        for (auto &text : m_optionTexts)
        {
            text.setCharacterSize(textSize);
            const sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                            bounds.position.y + bounds.size.y / 2.f});
        }

        const float lineSpacing = m_font.getLineSpacing(textSize);
        const float totalHeight = lineSpacing * static_cast<float>(m_optionTexts.size());
        const float padding = 18.f * textScale;
        const sf::Vector2f containerSize{672.f * textScale,
                                         std::max(125.f * textScale, totalHeight + padding * 2.f)};
        const sf::Vector2f containerCenter{viewCenter.x, viewCenter.y + viewSize.y * 0.13f};

        const float startY = containerCenter.y - totalHeight / 2.f + lineSpacing / 2.f;
        for (std::size_t i = 0; i < m_optionTexts.size(); ++i)
        {
            m_optionTexts[i].setPosition({containerCenter.x, startY + lineSpacing * static_cast<float>(i)});
        }

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

        m_menuGradient[0].position = {x0, top};
        m_menuGradient[1].position = {x0, bottom};
        m_menuGradient[2].position = {x1, top};
        m_menuGradient[3].position = {x1, bottom};
        m_menuGradient[4].position = {x2, top};
        m_menuGradient[5].position = {x2, bottom};
        m_menuGradient[6].position = {x3, top};
        m_menuGradient[7].position = {x3, bottom};

        m_menuGradient[0].color = transparent;
        m_menuGradient[1].color = transparent;
        m_menuGradient[2].color = opaque;
        m_menuGradient[3].color = opaque;
        m_menuGradient[4].color = opaque;
        m_menuGradient[5].color = opaque;
        m_menuGradient[6].color = transparent;
        m_menuGradient[7].color = transparent;
    }

    // Set the active option index. #
    void MenuScreen::setSelectedIndex(std::size_t index)
    {
        if (m_optionTexts.empty())
            return;

        m_selectedIndex = std::min(index, m_optionTexts.size() - 1);
        applyOptionColors(255);
    }

    // Apply colors to reflect selection and blink state. #
    void MenuScreen::applyOptionColors(std::uint8_t selectedAlpha)
    {
        for (std::size_t i = 0; i < m_optionTexts.size(); ++i)
        {
            if (i == m_selectedIndex)
            {
                sf::Color color = MENU_ACCENT_COLOR;
                color.a = selectedAlpha;
                m_optionTexts[i].setFillColor(color);
            }
            else
            {
                m_optionTexts[i].setFillColor(sf::Color::White);
            }
        }
    }

    // Move selection up or down with wrap-around. #
    void MenuScreen::selectNext(int direction)
    {
        if (m_optionTexts.empty())
            return;

        const int count = static_cast<int>(m_optionTexts.size());
        int next = static_cast<int>(m_selectedIndex) + direction;
        if (next < 0)
            next = count - 1;
        if (next >= count)
            next = 0;
        setSelectedIndex(static_cast<std::size_t>(next));
    }

    // Invoke the callback for the active option. #
    void MenuScreen::activateSelected()
    {
        if (!m_onOptionSelected || m_selectedIndex >= m_options.size())
            return;

        m_onOptionSelected(m_options[m_selectedIndex]);
    }
}

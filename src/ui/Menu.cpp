#include "ui/Menu.hpp"
#include "resources/ResourceLoader.hpp"
#include "ui/UiConstants.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>

namespace UI
{
    Menu::Menu()
        : m_backgroundSprite(m_backgroundTexture),
          m_screenBackdrop(sf::PrimitiveType::TriangleStrip, 4)
    {
    }

    bool Menu::initBackground(const std::string &path)
    {
        Resources::loadTextureInto(m_backgroundTexture,
                                   path,
                                   "Menu",
                                   path);

        m_backgroundSprite.setTexture(m_backgroundTexture, true);
        const sf::Vector2u size = m_backgroundTexture.getSize();
        m_backgroundSprite.setOrigin({static_cast<float>(size.x) / 2.f,
                                      static_cast<float>(size.y) / 2.f});
        m_hasBackground = true;
        return true;
    }

    void Menu::setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    void Menu::toggleEnabled()
    {
        m_enabled = !m_enabled;
    }

    bool Menu::isEnabled() const
    {
        return m_enabled;
    }

    void Menu::addButton(std::unique_ptr<StateComponent> button)
    {
        m_buttons.emplace_back(std::move(button));
    }

    void Menu::updateHover(const sf::RenderWindow &window, sf::Vector2i mousePos)
    {
        if (!m_enabled)
            return;

        const sf::View uiView = UI::makeLetterboxView(window.getSize());
        layout(window);

        const sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, uiView);
        for (const auto &button : m_buttons)
        {
            button->updateHover(worldPos);
        }
    }

    bool Menu::handleClick(const sf::RenderWindow &window, sf::Vector2i mousePos)
    {
        if (!m_enabled)
            return false;

        const sf::View uiView = UI::makeLetterboxView(window.getSize());
        layout(window);

        const sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, uiView);
        for (const auto &button : m_buttons)
        {
            if (button->handleClick(worldPos))
                return true;
        }
        return false;
    }

    void Menu::render(sf::RenderWindow &window)
    {
        if (!m_enabled)
            return;

        sf::View oldView = window.getView();
        sf::View uiView = UI::makeLetterboxView(window.getSize());
        window.setView(uiView);

        layout(window);

        if (m_screenBackdropEnabled)
            window.draw(m_screenBackdrop);

        if (m_hasBackground)
            window.draw(m_backgroundSprite);

        for (const auto &button : m_buttons)
        {
            button->render(window);
        }

        window.setView(oldView);
    }

    void Menu::layout(const sf::RenderWindow &window)
    {
        const sf::View view = UI::makeLetterboxView(window.getSize());
        const sf::Vector2f viewSize = view.getSize();
        const sf::Vector2f viewCenter = view.getCenter();

        updateScreenBackdrop(view);

        if (m_hasBackground)
        {
            const sf::Vector2u texSize = m_backgroundTexture.getSize();
            if (texSize.x > 0 && texSize.y > 0)
            {
                const float scaleX = viewSize.x / static_cast<float>(texSize.x);
                const float scaleY = viewSize.y / static_cast<float>(texSize.y);
                const float scale = std::min(scaleX, scaleY) * 0.85f;
                m_backgroundSprite.setScale({scale, scale});
                m_backgroundSprite.setPosition(viewCenter);
            }
        }

        if (m_buttons.empty())
            return;

        const float scale = std::min(viewSize.x / UI_DESIGN_SIZE.x,
                                     viewSize.y / UI_DESIGN_SIZE.y);
        const float gap = m_buttonGap * scale;

        float totalHeight = 0.f;
        std::vector<float> heights;
        heights.reserve(m_buttons.size());

        for (const auto &button : m_buttons)
        {
            button->setScale({scale, scale});
            const float height = button->bounds().size.y;
            heights.push_back(height);
            totalHeight += height;
        }

        if (m_buttons.size() > 1)
            totalHeight += gap * static_cast<float>(m_buttons.size() - 1);

        float currentY = viewCenter.y - totalHeight / 2.f;
        for (size_t i = 0; i < m_buttons.size(); ++i)
        {
            const float height = heights[i];
            m_buttons[i]->setPosition({viewCenter.x, currentY + height / 2.f});
            currentY += height + gap;
        }
    }

    void Menu::setScreenBackdropEnabled(bool enabled)
    {
        m_screenBackdropEnabled = enabled;
    }

    void Menu::setScreenBackdropColors(const sf::Color &top, const sf::Color &bottom)
    {
        m_screenBackdropTop = top;
        m_screenBackdropBottom = bottom;
    }

    void Menu::updateScreenBackdrop(const sf::View &view)
    {
        const sf::Vector2f viewSize = view.getSize();
        const sf::Vector2f viewCenter = view.getCenter();
        const sf::Vector2f topLeft{viewCenter.x - viewSize.x / 2.f,
                                   viewCenter.y - viewSize.y / 2.f};
        const sf::Vector2f topRight{topLeft.x + viewSize.x, topLeft.y};
        const sf::Vector2f bottomLeft{topLeft.x, topLeft.y + viewSize.y};
        const sf::Vector2f bottomRight{topLeft.x + viewSize.x, topLeft.y + viewSize.y};

        m_screenBackdrop[0].position = topLeft;
        m_screenBackdrop[1].position = topRight;
        m_screenBackdrop[2].position = bottomLeft;
        m_screenBackdrop[3].position = bottomRight;

        m_screenBackdrop[0].color = m_screenBackdropTop;
        m_screenBackdrop[1].color = m_screenBackdropTop;
        m_screenBackdrop[2].color = m_screenBackdropBottom;
        m_screenBackdrop[3].color = m_screenBackdropBottom;
    }
}

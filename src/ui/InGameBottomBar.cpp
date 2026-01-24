#include "ui/InGameBottomBar.hpp"
#include "audio/SfxManager.hpp"
#include "resources/ResourceLoader.hpp"
#include "ui/UiConstants.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>
#include <cmath>

namespace UI
{
    namespace
    {
        constexpr const char *HAMBURGER_ICON_PATH = "assets/ui/menu-icon.png";
        constexpr const char *W_ICON_PATH = "assets/ui/w_key_icon.png";
        constexpr const char *R_ICON_PATH = "assets/ui/r_key_icon.png";
        constexpr const char *W_LABEL_TEXT = "BUILD WALLS";
        constexpr const char *R_LABEL_TEXT = "ROTATE WALL";
        const sf::Color HOVER_COLOR(220, 220, 220);
        const sf::Color WALL_MODE_ACTIVE_COLOR(200, 200, 200, 180);
    }

    InGameBottomBar::InGameBottomBar()
        : m_hamburgerSprite(m_hamburgerTexture),
          m_wIconSprite(m_wIconTexture),
          m_rIconSprite(m_rIconTexture),
          m_wLabel(m_font),
          m_rLabel(m_font)
    {
    }

    bool InGameBottomBar::init()
    {
        Resources::loadTextureInto(m_hamburgerTexture,
                                   HAMBURGER_ICON_PATH,
                                   "InGameBottomBar",
                                   "Menu icon");

        m_hamburgerSprite.setTexture(m_hamburgerTexture, true);
        const sf::Vector2u texSize = m_hamburgerTexture.getSize();
        m_hamburgerSprite.setOrigin({static_cast<float>(texSize.x) / 2.f,
                                     static_cast<float>(texSize.y) / 2.f});

        Resources::loadTextureInto(m_wIconTexture,
                                   W_ICON_PATH,
                                   "InGameBottomBar",
                                   "W key icon");

        Resources::loadTextureInto(m_rIconTexture,
                                   R_ICON_PATH,
                                   "InGameBottomBar",
                                   "R key icon");

        m_wIconSprite.setTexture(m_wIconTexture, true);
        m_rIconSprite.setTexture(m_rIconTexture, true);
        const sf::Vector2u wTexSize = m_wIconTexture.getSize();
        const sf::Vector2u rTexSize = m_rIconTexture.getSize();
        m_wIconSprite.setOrigin({static_cast<float>(wTexSize.x) / 2.f,
                                 static_cast<float>(wTexSize.y) / 2.f});
        m_rIconSprite.setOrigin({static_cast<float>(rTexSize.x) / 2.f,
                                 static_cast<float>(rTexSize.y) / 2.f});

        Resources::loadFontInto(m_font,
                                "assets/fonts/pixelon.ttf",
                                "InGameBottomBar",
                                "UI font");
        m_hasFont = true;
        m_wLabel.setFont(m_font);
        m_rLabel.setFont(m_font);
        m_wLabel.setString(W_LABEL_TEXT);
        m_rLabel.setString(R_LABEL_TEXT);
        m_wLabel.setFillColor(sf::Color::White);
        m_rLabel.setFillColor(sf::Color::White);

        updateWallIndicatorColors();
        return true;
    }

    void InGameBottomBar::setOnClick(std::function<void()> onClick)
    {
        m_onClick = std::move(onClick);
    }

    void InGameBottomBar::setOnToggleWallMode(std::function<void()> onToggleWallMode)
    {
        m_onToggleWallMode = std::move(onToggleWallMode);
    }

    void InGameBottomBar::setOnRotateWall(std::function<void()> onRotateWall)
    {
        m_onRotateWall = std::move(onRotateWall);
    }

    void InGameBottomBar::setWallPlacementActive(bool active)
    {
        if (m_wallModeActive == active)
            return;

        m_wallModeActive = active;
        if (!m_wallModeActive)
            m_rHovered = false;
        updateWallIndicatorColors();
    }

    bool InGameBottomBar::handleEvent(const sf::Event &event, sf::RenderWindow &window)
    {
        if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>())
        {
            updateLayout(window);
            const sf::Vector2f worldPos = window.mapPixelToCoords(mouseMove->position, m_view);
            updateHoverState(worldPos);
            return m_barBounds.contains(worldPos);
        }

        if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseBtn->button != sf::Mouse::Button::Left)
                return false;

            updateLayout(window);
            const sf::Vector2f worldPos = window.mapPixelToCoords(mouseBtn->position, m_view);
            if (m_wControlBounds.contains(worldPos))
            {
                Audio::SfxManager::instance().play(Audio::SfxId::Click);
                if (m_onToggleWallMode)
                    m_onToggleWallMode();
                return true;
            }
            if (m_wallModeActive && m_rControlBounds.contains(worldPos))
            {
                Audio::SfxManager::instance().play(Audio::SfxId::Click);
                if (m_onRotateWall)
                    m_onRotateWall();
                return true;
            }
            if (m_hamburgerBounds.contains(worldPos))
            {
                Audio::SfxManager::instance().play(Audio::SfxId::Click);
                if (m_onClick)
                    m_onClick();
                return true;
            }

            if (m_barBounds.contains(worldPos))
                return true;
        }

        return false;
    }

    void InGameBottomBar::render(sf::RenderWindow &window)
    {
        updateLayout(window);

        sf::View oldView = window.getView();
        window.setView(m_view);

        window.draw(m_bar);
        window.draw(m_wIconSprite);
        if (m_wallModeActive)
            window.draw(m_rIconSprite);
        if (m_hasFont)
        {
            window.draw(m_wLabel);
            if (m_wallModeActive)
                window.draw(m_rLabel);
        }
        window.draw(m_hamburgerSprite);

        window.setView(oldView);
    }

    void InGameBottomBar::resetHover()
    {
        m_hovered = false;
        m_wHovered = false;
        m_rHovered = false;
        applyHoverColor();
    }

    void InGameBottomBar::updateLayout(const sf::RenderWindow &window)
    {
        m_view = UI::makeLetterboxView(window.getSize());
        m_viewSize = m_view.getSize();
        const sf::Vector2f viewCenter = m_view.getCenter();
        m_viewTopLeft = {viewCenter.x - m_viewSize.x / 2.f,
                         viewCenter.y - m_viewSize.y / 2.f};

        m_layoutScale = std::min(m_viewSize.x / UI_DESIGN_SIZE.x,
                                 m_viewSize.y / UI_DESIGN_SIZE.y);
        m_barHeight = UI::INGAME_BOTTOM_BAR_HEIGHT * m_layoutScale;
        m_barPadding = UI::INGAME_MENU_ICON_PADDING * m_layoutScale; // <BAR_PADDING_PX>

        const sf::Vector2f iconSize = {m_viewSize.x * CONTROL_ICON_W_RATIO,
                                       m_viewSize.y * CONTROL_ICON_H_RATIO};

        m_bar.setSize({m_viewSize.x, m_barHeight});
        m_bar.setPosition({m_viewTopLeft.x, m_viewTopLeft.y + m_viewSize.y - m_barHeight});
        m_bar.setFillColor(sf::Color(0, 0, 0, 204));

        const sf::Vector2u texSize = m_hamburgerTexture.getSize();
        if (texSize.x > 0 && texSize.y > 0)
        {
            m_hamburgerSprite.setScale({iconSize.x / static_cast<float>(texSize.x),
                                        iconSize.y / static_cast<float>(texSize.y)});
        }

        const float iconCenterX = m_viewTopLeft.x + m_viewSize.x - m_barPadding - iconSize.x / 2.f;
        const float iconCenterY = m_bar.getPosition().y + m_barHeight / 2.f;
        m_hamburgerSprite.setPosition({iconCenterX, iconCenterY});

        m_barBounds = m_bar.getGlobalBounds();
        m_hamburgerBounds = m_hamburgerSprite.getGlobalBounds();

        layoutControlsIcons();
    }

    void InGameBottomBar::updateHoverState(const sf::Vector2f &worldPos)
    {
        const bool hovered = m_hamburgerBounds.contains(worldPos);
        const bool wHovered = m_wControlBounds.contains(worldPos);
        const bool rHovered = m_wallModeActive && m_rControlBounds.contains(worldPos);
        if (hovered == m_hovered && wHovered == m_wHovered && rHovered == m_rHovered)
            return;

        if (hovered && !m_hovered)
            Audio::SfxManager::instance().play(Audio::SfxId::Hover);
        if (wHovered && !m_wHovered)
            Audio::SfxManager::instance().play(Audio::SfxId::Hover);
        if (rHovered && !m_rHovered)
            Audio::SfxManager::instance().play(Audio::SfxId::Hover);

        m_hovered = hovered;
        m_wHovered = wHovered;
        m_rHovered = rHovered;
        applyHoverColor();
    }

    void InGameBottomBar::applyHoverColor()
    {
        m_hamburgerSprite.setColor(m_hovered ? HOVER_COLOR : sf::Color::White);
        const sf::Color wBase = m_wallModeActive ? WALL_MODE_ACTIVE_COLOR : sf::Color::White;
        const sf::Color wColor = m_wHovered ? HOVER_COLOR : wBase;
        const sf::Color rColor = m_rHovered ? HOVER_COLOR : sf::Color::White;
        const sf::Color rHidden(255, 255, 255, 0);

        m_wIconSprite.setColor(wColor);
        if (m_hasFont)
            m_wLabel.setFillColor(wColor);

        if (m_wallModeActive)
        {
            m_rIconSprite.setColor(rColor);
            if (m_hasFont)
                m_rLabel.setFillColor(rColor);
        }
        else
        {
            m_rIconSprite.setColor(rHidden);
            if (m_hasFont)
                m_rLabel.setFillColor(rHidden);
        }
    }

    void InGameBottomBar::updateWallIndicatorColors()
    {
        applyHoverColor();
    }

    void InGameBottomBar::layoutControlsIcons()
    {
        const bool showRotate = m_wallModeActive;
        float iconWidth = m_viewSize.x * CONTROL_ICON_W_RATIO;
        float iconHeight = m_viewSize.y * CONTROL_ICON_H_RATIO;
        float groupGap = CONTROL_ICON_GAP_PX * m_layoutScale;
        float labelGap = CONTROL_LABEL_GAP_PX * m_layoutScale;

        unsigned int fontSize = 1;
        if (m_hasFont)
        {
            fontSize = std::max(1u, static_cast<unsigned int>(
                                        std::round(CONTROL_LABEL_FONT_SIZE * m_layoutScale)));
            m_wLabel.setCharacterSize(fontSize);
            m_rLabel.setCharacterSize(fontSize);
        }

        auto measureWidth = [](const sf::Text &text)
        {
            const sf::FloatRect bounds = text.getLocalBounds();
            return bounds.size.x;
        };

        float wLabelWidth = m_hasFont ? measureWidth(m_wLabel) : 0.f;
        float rLabelWidth = (m_hasFont && showRotate) ? measureWidth(m_rLabel) : 0.f;

        float group1Width = iconWidth + (m_hasFont ? (labelGap + wLabelWidth) : 0.f);
        float group2Width = showRotate ? (iconWidth + (m_hasFont ? (labelGap + rLabelWidth) : 0.f)) : 0.f;
        float totalWidth = group1Width + (showRotate ? (groupGap + group2Width) : 0.f);

        const float leftX = m_bar.getPosition().x + m_barPadding;
        const float maxRight = m_hamburgerBounds.position.x - m_barPadding;
        const float available = maxRight - leftX;

        if (available <= 0.f)
        {
            m_wIconSprite.setScale({0.f, 0.f});
            m_rIconSprite.setScale({0.f, 0.f});
            m_wIconBounds = {};
            m_rIconBounds = {};
            m_wControlBounds = {};
            m_rControlBounds = {};
            return;
        }

        if (totalWidth > available)
        {
            const float fitScale = std::max(available / totalWidth, 0.f);
            iconWidth *= fitScale;
            iconHeight *= fitScale;
            groupGap *= fitScale;
            labelGap *= fitScale;

            if (m_hasFont)
            {
                fontSize = std::max(1u, static_cast<unsigned int>(
                                            std::round(CONTROL_LABEL_FONT_SIZE * m_layoutScale * fitScale)));
                m_wLabel.setCharacterSize(fontSize);
                m_rLabel.setCharacterSize(fontSize);
                wLabelWidth = measureWidth(m_wLabel);
                rLabelWidth = (m_hasFont && showRotate) ? measureWidth(m_rLabel) : 0.f;
            }

            group1Width = iconWidth + (m_hasFont ? (labelGap + wLabelWidth) : 0.f);
            group2Width = showRotate ? (iconWidth + (m_hasFont ? (labelGap + rLabelWidth) : 0.f)) : 0.f;
        }

        const float centerY = m_bar.getPosition().y + m_barHeight / 2.f;

        const sf::Vector2u wTexSize = m_wIconTexture.getSize();
        const sf::Vector2u rTexSize = m_rIconTexture.getSize();
        if (wTexSize.x > 0 && wTexSize.y > 0)
        {
            m_wIconSprite.setScale({iconWidth / static_cast<float>(wTexSize.x),
                                    iconHeight / static_cast<float>(wTexSize.y)});
        }
        if (rTexSize.x > 0 && rTexSize.y > 0)
        {
            m_rIconSprite.setScale({iconWidth / static_cast<float>(rTexSize.x),
                                    iconHeight / static_cast<float>(rTexSize.y)});
        }

        const float wIconCenterX = leftX + iconWidth / 2.f;
        m_wIconSprite.setPosition({wIconCenterX, centerY});

        if (showRotate)
        {
            const float rGroupStartX = leftX + group1Width + groupGap;
            const float rIconCenterX = rGroupStartX + iconWidth / 2.f;
            m_rIconSprite.setPosition({rIconCenterX, centerY});
        }

        if (m_hasFont)
        {
            auto setLeftCentered = [](sf::Text &text, float x, float y)
            {
                const sf::FloatRect bounds = text.getLocalBounds();
                text.setOrigin({bounds.position.x,
                                bounds.position.y + bounds.size.y / 2.f});
                text.setPosition({x, y});
            };

            const float wLabelX = wIconCenterX + iconWidth / 2.f + labelGap;
            setLeftCentered(m_wLabel, wLabelX, centerY);

            if (showRotate)
            {
                const float rGroupStartX = leftX + group1Width + groupGap;
                const float rIconCenterX = rGroupStartX + iconWidth / 2.f;
                const float rLabelX = rIconCenterX + iconWidth / 2.f + labelGap;
                setLeftCentered(m_rLabel, rLabelX, centerY);
            }
        }

        m_wIconBounds = m_wIconSprite.getGlobalBounds();
        m_rIconBounds = showRotate ? m_rIconSprite.getGlobalBounds() : sf::FloatRect();

        m_wControlBounds = m_wIconBounds;
        if (m_hasFont)
        {
            const sf::FloatRect wLabelBounds = m_wLabel.getGlobalBounds();
            const float left = std::min(m_wControlBounds.position.x, wLabelBounds.position.x);
            const float top = std::min(m_wControlBounds.position.y, wLabelBounds.position.y);
            const float right = std::max(m_wControlBounds.position.x + m_wControlBounds.size.x,
                                         wLabelBounds.position.x + wLabelBounds.size.x);
            const float bottom = std::max(m_wControlBounds.position.y + m_wControlBounds.size.y,
                                          wLabelBounds.position.y + wLabelBounds.size.y);
            m_wControlBounds = sf::FloatRect({left, top}, {right - left, bottom - top});
        }

        if (showRotate)
        {
            m_rControlBounds = m_rIconBounds;
            if (m_hasFont)
            {
                const sf::FloatRect rLabelBounds = m_rLabel.getGlobalBounds();
                const float left = std::min(m_rControlBounds.position.x, rLabelBounds.position.x);
                const float top = std::min(m_rControlBounds.position.y, rLabelBounds.position.y);
                const float right = std::max(m_rControlBounds.position.x + m_rControlBounds.size.x,
                                             rLabelBounds.position.x + rLabelBounds.size.x);
                const float bottom = std::max(m_rControlBounds.position.y + m_rControlBounds.size.y,
                                              rLabelBounds.position.y + rLabelBounds.size.y);
                m_rControlBounds = sf::FloatRect({left, top}, {right - left, bottom - top});
            }
        }
        else
        {
            m_rControlBounds = {};
        }
    }
}

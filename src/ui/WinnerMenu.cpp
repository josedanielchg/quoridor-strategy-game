#include "ui/WinnerMenu.hpp"
#include "ui/UiConstants.hpp"
#include "ui/ViewUtils.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace UI
{
    namespace
    {
        // Resolve banner path based on winner id. #
        const char *bannerPathForPlayer(int playerId)
        {
            return (playerId == 2) ? "assets/textures/ui_banner_win_p2.png"
                                   : "assets/textures/ui_banner_win_p1.png";
        }
    }

    // Initialize menu buttons and winner banner. #
    bool WinnerMenu::init()
    {
        setScreenBackdropEnabled(true);
        setScreenBackdropColors(sf::Color(0, 0, 0, 150),
                                sf::Color(0, 0, 0, 210));

        auto restart = std::make_unique<Button>();
        if (!restart->init("assets/textures/button.png",
                           std::string(),
                           "assets/fonts/pixelon.ttf",
                           "Restart"))
        {
            std::cerr << "Failed to load Restart button\n";
            return false;
        }
        m_restartButton = restart.get();
        addButton(std::move(restart));

        auto quit = std::make_unique<Button>();
        if (!quit->init("assets/textures/button.png",
                        std::string(),
                        "assets/fonts/pixelon.ttf",
                        "Quit"))
        {
            std::cerr << "Failed to load Quit button\n";
            return false;
        }
        m_quitButton = quit.get();
        addButton(std::move(quit));

        if (!initBackground(bannerPathForPlayer(m_winnerId)))
        {
            std::cerr << "Failed to load winner banner\n";
            return false;
        }

        return true;
    }

    // Update the winner id and reload banner texture. #
    void WinnerMenu::setWinner(int playerId)
    {
        if (playerId == m_winnerId && m_hasBackground)
            return;

        m_winnerId = playerId;
        if (!initBackground(bannerPathForPlayer(m_winnerId)))
        {
            std::cerr << "Failed to load winner banner\n";
            m_hasBackground = false;
        }
    }

    // Set restart button action. #
    void WinnerMenu::setOnRestart(std::function<void()> action)
    {
        if (m_restartButton)
            m_restartButton->setOnClick(std::move(action));
    }

    // Set quit button action. #
    void WinnerMenu::setOnQuit(std::function<void()> action)
    {
        if (m_quitButton)
            m_quitButton->setOnClick(std::move(action));
    }

    // Layout banner and buttons relative to the view. #
    void WinnerMenu::layout(const sf::RenderWindow &window)
    {
        const sf::View view = UI::makeLetterboxView(window.getSize());
        const sf::Vector2f viewSize = view.getSize();
        const sf::Vector2f viewCenter = view.getCenter();
        const sf::Vector2f raisedCenter = {viewCenter.x,
                                           viewCenter.y - viewSize.y * 0.05f};

        updateScreenBackdrop(view);

        sf::Vector2f bannerSize = UI::WIN_BANNER_SIZE;
        if (m_hasBackground)
        {
            const sf::Vector2u texSize = m_backgroundTexture.getSize();
            if (texSize.x > 0 && texSize.y > 0)
            {
                const float scale = std::min(viewSize.x / UI_DESIGN_SIZE.x,
                                             viewSize.y / UI_DESIGN_SIZE.y);
                bannerSize = {UI::WIN_BANNER_SIZE.x * scale,
                              UI::WIN_BANNER_SIZE.y * scale};
                m_backgroundSprite.setScale({bannerSize.x / static_cast<float>(texSize.x),
                                             bannerSize.y / static_cast<float>(texSize.y)});
                m_backgroundSprite.setPosition(raisedCenter);
            }
        }

        if (m_buttons.size() < 2)
            return;

        const float viewScale = std::min(viewSize.x / UI_DESIGN_SIZE.x,
                                         viewSize.y / UI_DESIGN_SIZE.y);
        const float spanScale = UI::WIN_BANNER_BUTTON_SPAN / UI::WIN_BANNER_SIZE.x;
        const float buttonSpan = bannerSize.x * spanScale;
        const float buttonWidth = buttonSpan * 0.48f;
        const float horizontalGap = buttonSpan * 0.04f;
        const float verticalGap = UI::WIN_BANNER_BUTTON_GAP * viewScale;

        for (const auto &button : m_buttons)
        {
            button->setScale({1.f, 1.f});
        }

        const float baseWidth = m_buttons[0]->size().x;
        const float buttonScale = (baseWidth > 0.f) ? (buttonWidth / baseWidth) : 1.f;
        for (const auto &button : m_buttons)
        {
            button->setScale({buttonScale, buttonScale});
        }

        const float buttonHeight = m_buttons[0]->size().y;
        const float bannerBottom = raisedCenter.y + bannerSize.y / 2.f;
        const float buttonY = bannerBottom + verticalGap + buttonHeight / 2.f;
        const float leftX = raisedCenter.x - (buttonWidth / 2.f + horizontalGap / 2.f);
        const float rightX = raisedCenter.x + (buttonWidth / 2.f + horizontalGap / 2.f);

        m_buttons[0]->setPosition({leftX, buttonY});
        m_buttons[1]->setPosition({rightX, buttonY});
    }
}

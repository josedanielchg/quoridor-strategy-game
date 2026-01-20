#include "ui/PauseMenu.hpp"
#include "ui/UiConstants.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace UI
{
    bool PauseMenu::init()
    {
        if (!initBackground("assets/textures/pause-background.png"))
        {
            std::cerr << "Failed to load pause background\n";
            return false;
        }

        setScreenBackdropEnabled(true);
        setScreenBackdropColors(sf::Color(0, 0, 0, 140),
                                sf::Color(0, 0, 0, 210));

        auto resume = std::make_unique<Button>();
        if (!resume->init("assets/textures/button.png",
                          std::string(),
                          "assets/fonts/roboto.ttf",
                          "Resume"))
        {
            std::cerr << "Failed to load Resume button\n";
            return false;
        }
        m_resumeButton = resume.get();
        addButton(std::move(resume));

        auto restart = std::make_unique<Button>();
        if (!restart->init("assets/textures/button.png",
                           std::string(),
                           "assets/fonts/roboto.ttf",
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
                        "assets/fonts/roboto.ttf",
                        "Quit"))
        {
            std::cerr << "Failed to load Quit button\n";
            return false;
        }
        m_quitButton = quit.get();
        addButton(std::move(quit));

        if (m_resumeButton)
            m_resumeButton->setOnClick([this]() { setEnabled(false); });

        return true;
    }

    void PauseMenu::setOnResume(std::function<void()> action)
    {
        if (m_resumeButton)
            m_resumeButton->setOnClick(std::move(action));
    }

    void PauseMenu::setOnRestart(std::function<void()> action)
    {
        if (m_restartButton)
            m_restartButton->setOnClick(std::move(action));
    }

    void PauseMenu::setOnQuit(std::function<void()> action)
    {
        if (m_quitButton)
            m_quitButton->setOnClick(std::move(action));
    }

    void PauseMenu::layout(const sf::RenderWindow &window)
    {
        const sf::View view = window.getDefaultView();
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

        sf::FloatRect anchorBounds;
        if (m_hasBackground)
        {
            anchorBounds = m_backgroundSprite.getGlobalBounds();
        }
        else
        {
            anchorBounds = sf::FloatRect({viewCenter.x - viewSize.x / 2.f,
                                          viewCenter.y - viewSize.y / 2.f},
                                         viewSize);
        }

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

        const float centerX = anchorBounds.position.x + anchorBounds.size.x / 2.f;
        float currentY = anchorBounds.position.y + (anchorBounds.size.y - totalHeight) / 2.f;

        for (size_t i = 0; i < m_buttons.size(); ++i)
        {
            const float height = heights[i];
            m_buttons[i]->setPosition({centerX, currentY + height / 2.f});
            currentY += height + gap;
        }
    }
}

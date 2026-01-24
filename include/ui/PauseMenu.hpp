#pragma once
#include <functional>
#include "ui/Button.hpp"
#include "ui/Menu.hpp"

namespace UI
{
    class PauseMenu : public Menu
    {
    public:
        bool init(); // Build pause menu #

        void setOnResume(std::function<void()> action); // Register resume action #
        void setOnRestart(std::function<void()> action); // Register restart action #
        void setOnQuit(std::function<void()> action); // Register quit action #

    private:
        void layout(const sf::RenderWindow &window) override; // Layout buttons #

        Button *m_resumeButton = nullptr;
        Button *m_restartButton = nullptr;
        Button *m_quitButton = nullptr;

    protected:
        float m_buttonGap = 16.f;
    };
}

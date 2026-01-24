#pragma once
#include <functional>
#include "ui/Button.hpp"
#include "ui/Menu.hpp"

namespace UI
{
    class WinnerMenu : public Menu
    {
    public:
        bool init(); // Build winner menu #
        void setWinner(int playerId); // Update winner banner #
        void setOnRestart(std::function<void()> action); // Register restart action #
        void setOnQuit(std::function<void()> action); // Register quit action #

    private:
        void layout(const sf::RenderWindow &window) override; // Layout buttons and banner #

        Button *m_restartButton = nullptr;
        Button *m_quitButton = nullptr;
        int m_winnerId = 1;
    };
}

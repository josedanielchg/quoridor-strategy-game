#pragma once
#include <functional>
#include "ui/Button.hpp"
#include "ui/Menu.hpp"

namespace UI
{
    class WinnerMenu : public Menu
    {
    public:
        bool init();
        void setWinner(int playerId);
        void setOnRestart(std::function<void()> action);
        void setOnQuit(std::function<void()> action);

    private:
        void layout(const sf::RenderWindow &window) override;

        Button *m_restartButton = nullptr;
        Button *m_quitButton = nullptr;
        int m_winnerId = 1;
    };
}

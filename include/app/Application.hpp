#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "app/MenuScreen.hpp"
#include "app/CreditsScreen.hpp"
#include "app/HowToPlayScreen.hpp"
#include "app/GameScreen.hpp"
#include "app/Screen.hpp"
#include "app/TitleScreen.hpp"

namespace App
{

    class Application
    {
    public:
        // Build window, initialize screens, and wire navigation callbacks.
        Application();
        // Run the main loop until the window closes.
        void run();

    private:
        // Pump OS/window events and forward to the active screen.
        void processEvents();
        // Advance the active screen simulation for the current frame.
        void update();
        // Draw the active screen to the window.
        void render();
        // Switch active screen and run enter/exit hooks.
        void setCurrentScreen(Screen *screen);

    private:
        sf::RenderWindow m_window;
        sf::Clock m_clock;
        std::unique_ptr<TitleScreen> m_titleScreen;
        std::unique_ptr<MenuScreen> m_menuScreen;
        std::unique_ptr<CreditsScreen> m_creditsScreen;
        std::unique_ptr<HowToPlayScreen> m_howToPlayScreen;
        std::unique_ptr<GameScreen> m_gameScreen;
        Screen *m_currentScreen = nullptr;
    };

}

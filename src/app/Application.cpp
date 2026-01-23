#include "app/Application.hpp"
#include "game/GameRules.hpp"
#include "game/Move.hpp"
#include <iostream>
#include <optional>

namespace App
{

    Application::Application()
        : m_window(sf::VideoMode({1280, 720}), "Quoridor Isometric")
{
        m_window.setFramerateLimit(60);

        m_titleScreen = std::make_unique<TitleScreen>();
        if (!m_titleScreen->init())
            std::cerr << "Warning: Title screen failed\n";

        m_menuScreen = std::make_unique<MenuScreen>();
        if (!m_menuScreen->init())
            std::cerr << "Warning: Menu screen failed\n";

        m_creditsScreen = std::make_unique<CreditsScreen>();
        if (!m_creditsScreen->init())
            std::cerr << "Warning: Credits screen failed\n";

        m_howToPlayScreen = std::make_unique<HowToPlayScreen>();
        if (!m_howToPlayScreen->init())
            std::cerr << "Warning: How-to-play screen failed\n";

        m_gameScreen = std::make_unique<GameScreen>();
        if (!m_gameScreen->init())
            std::cerr << "Warning: Game screen failed\n";

        m_titleScreen->setOnStart([this]() { setCurrentScreen(m_menuScreen.get()); });
        m_creditsScreen->setOnBack([this]() { setCurrentScreen(m_menuScreen.get()); });
        m_howToPlayScreen->setOnBack([this]() { setCurrentScreen(m_menuScreen.get()); });
        m_menuScreen->setOnOptionSelected([this](MenuScreen::Option option)
                                          {
                                              if (option == MenuScreen::Option::SinglePlayer)
                                              {
                                                  m_gameScreen->setGameMode(GameScreen::GameMode::SinglePlayer);
                                                  setCurrentScreen(m_gameScreen.get());
                                              }
                                              else if (option == MenuScreen::Option::Multiplayer)
                                              {
                                                  m_gameScreen->setGameMode(GameScreen::GameMode::Multiplayer);
                                                  setCurrentScreen(m_gameScreen.get());
                                              }
                                              else if (option == MenuScreen::Option::HowToPlay)
                                              {
                                                  setCurrentScreen(m_howToPlayScreen.get());
                                              }
                                              else if (option == MenuScreen::Option::Credits)
                                              {
                                                  setCurrentScreen(m_creditsScreen.get());
                                              }
                                              else
                                              {
                                                  std::cout << "Menu option not wired yet\n";
                                              }
                                          });
        m_gameScreen->setOnQuit([this]() { setCurrentScreen(m_titleScreen.get()); });

        setCurrentScreen(m_titleScreen.get());
    }

    void Application::processEvents()
    {
        while (const std::optional event = m_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                m_window.close();
                continue;
            }
            if (const auto *resized = event->getIf<sf::Event::Resized>())
            {
                if (m_currentScreen)
                    m_currentScreen->handleResize(m_window, resized->size);
                continue;
            }

            if (m_currentScreen)
                m_currentScreen->handleEvent(*event, m_window);
        }
    }

    void Application::update()
    {
        const float dt = m_clock.restart().asSeconds();
        if (m_currentScreen)
            m_currentScreen->update(dt);
    }

    void Application::render()
    {
        m_window.clear(sf::Color::Black);

        if (m_currentScreen)
            m_currentScreen->render(m_window);

        m_window.display();
    }

    void Application::run()
    {
        while (m_window.isOpen())
        {
            processEvents();
            update();
            render();
        }
    }

    void Application::setCurrentScreen(Screen *screen)
    {
        if (m_currentScreen == screen)
            return;

        if (m_currentScreen)
            m_currentScreen->onExit();

        m_currentScreen = screen;
        if (m_currentScreen)
        {
            m_currentScreen->handleResize(m_window, m_window.getSize());
            m_currentScreen->onEnter();
        }
    }
}

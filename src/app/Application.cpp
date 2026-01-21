#include "app/Application.hpp"
#include <cstdlib>
#include <iostream>
#include <optional>

namespace App
{

    Application::Application()
        : m_window(sf::VideoMode({1280, 720}), "Quoridor Isometric")
    {
        m_window.setFramerateLimit(60);

        m_gameScreen = std::make_unique<GameScreen>();
        m_gameScreen->setOnQuit([this]() { m_window.close(); });
        if (!m_gameScreen->init())
            std::exit(-1);

        m_titleScreen = std::make_unique<TitleScreen>();
        if (!m_titleScreen->init())
            std::cerr << "Warning: Title screen failed\n";
        m_titleScreen->setOnStart([this]() { setCurrentScreen(m_gameScreen.get()); });

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
            }

            if (m_currentScreen)
                m_currentScreen->handleEvent(*event, m_window);
        }
    }

    void Application::render()
    {
        m_window.clear(sf::Color(30, 30, 30));

        if (m_currentScreen)
            m_currentScreen->render(m_window);

        m_window.display();
    }

    void Application::update()
    {
        const float dt = m_clock.restart().asSeconds();
        if (m_currentScreen)
            m_currentScreen->update(dt);
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
        m_currentScreen = screen;
        if (m_currentScreen)
            m_currentScreen->handleResize(m_window, m_window.getSize());
    }
}

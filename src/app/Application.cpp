#include "app/Application.hpp"
#include <iostream>

namespace App
{

    Application::Application()
        : m_window(sf::VideoMode({1280, 720}), "Quoridor Isometric")
    {
        m_window.setFramerateLimit(60);

        // 1. Init Logic
        m_board.init(); // Creates graph

        // 2. Init Resources (Textures)
        if (!m_renderer.init())
        {
            std::cerr << "Failed to load assets. Exiting.\n";
            exit(-1);
        }

        // 3. Place Players for Visualization
        // Player 1 at Top (4, 0)
        m_board.getField(4, 8)->setOccupantId(1);
        
        // Player 2 at Bottom (4, 8)
        m_board.getField(4, 0)->setOccupantId(2);

        // Initialize View
        m_renderer.handleResize(m_window, m_window.getSize());
    }

    void Application::processEvents()
    {
        while (const std::optional event = m_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                m_window.close();
            }
            else if (const auto *resized = event->getIf<sf::Event::Resized>())
            {
                m_renderer.handleResize(m_window, resized->size);
            }
            else if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Escape)
                    m_window.close();
            }
        }
    }

    void Application::render()
    {
        m_window.clear(sf::Color(30, 30, 30)); // Dark background

        // Pass the model to the view
        m_renderer.render(m_window, m_board);

        m_window.display();
    }

    void Application::update()
    {
        // Game logic updates go here
        static float time = 0.0f;
        time += 0.1f;
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
}
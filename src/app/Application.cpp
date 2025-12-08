#include "app/Application.hpp"
#include <iostream>

namespace App
{

    Application::Application()
        : m_window(sf::VideoMode({1280, 720}), "Quoridor Isometric")
    {
        m_window.setFramerateLimit(60);
        m_board.init(); // This now creates the pawns internally

        if (!m_renderer.init())
            exit(-1);
        if (!m_hud.init())
            std::cerr << "Warning: HUD failed\n";

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
                if (key->scancode == sf::Keyboard::Scancode::W)
                    toggleWallMode();
                if (key->scancode == sf::Keyboard::Scancode::R)
                    rotateWall();
            }

            // --- MOUSE MOVED (Hover Effect) ---
            else if (const auto *mouseMove = event->getIf<sf::Event::MouseMoved>())
            {
                sf::Vector2i gridPos = m_renderer.getMouseGridPos(m_window, mouseMove->position);

                if (m_isPlacingWall)
                {
                    m_renderer.setHoveredTile({-1, -1});
                    m_renderer.setWallPreview(true, gridPos, m_currentWallOri);
                }
                else
                {
                    m_renderer.setHoveredTile(gridPos);
                    m_renderer.setWallPreview(false, {0, 0}, m_currentWallOri);
                }
            }

            // --- MOUSE CLICK (Movement) ---
            else if (const auto *mouseBtn = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseBtn->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i gridPos = m_renderer.getMouseGridPos(m_window, mouseBtn->position);

                    if (m_isPlacingWall)
                    {
                        attemptPlaceWall(gridPos);
                    }
                    else
                    {
                        attemptMove(gridPos);
                    }
                }
            }
        }
    }

    void Application::attemptMove(sf::Vector2i gridPos)
    {
        // 1. Basic Validation
        if (gridPos.x == -1 || gridPos.y == -1)
            return;

        // 2. Delegate to Board/Pawn logic
        if (m_board.movePawn(m_currentPlayer, gridPos.x, gridPos.y))
        {
            // If successful:
            m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
            m_hud.update(m_currentPlayer);
        }
        else
        {
            std::cout << "Invalid Move!" << std::endl;
        }
    }

    void Application::render()
    {
        m_window.clear(sf::Color(30, 30, 30));

        // 1. Draw World
        m_renderer.render(m_window, m_board);

        // 2. Draw HUD (On top)
        m_hud.render(m_window);

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

    void Application::toggleWallMode()
    {
        m_isPlacingWall = !m_isPlacingWall;
        std::cout << "Wall Mode: " << (m_isPlacingWall ? "ON" : "OFF") << std::endl;
    }

    void Application::rotateWall()
    {
        if (m_currentWallOri == Game::Orientation::Horizontal)
            m_currentWallOri = Game::Orientation::Vertical;
        else
            m_currentWallOri = Game::Orientation::Horizontal;
    }

    void Application::attemptPlaceWall(sf::Vector2i gridPos)
    {
        if (gridPos.x == -1)
            return;

        bool success = m_board.placeWall(gridPos.x, gridPos.y, m_currentWallOri);

        if (success)
        {
            std::cout << "Wall placed at " << gridPos.x << ", " << gridPos.y << std::endl;
            m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
            m_hud.update(m_currentPlayer);
            m_isPlacingWall = false;
        }
        else
        {
            std::cout << "Invalid Wall Position!" << std::endl;
        }
    }
}
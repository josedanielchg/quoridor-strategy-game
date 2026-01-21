#include "app/Application.hpp"
#include "game/Move.hpp"
#include <iostream>

namespace App
{

    Application::Application()
        : m_window(sf::VideoMode({1280, 720}), "Quoridor Isometric")
    {
        m_window.setFramerateLimit(60);
        m_board.init();
        Game::initGameState(m_gameState);
        if (!m_board.loadFromState(m_gameState))
        {
            std::cerr << "Error: Failed to load initial game state into board.\n";
            m_window.close();
            return;
        }

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
        // 1. Check if game is already won
        if (m_gameState.isGameOver())
        {
            std::cout << "Game Over! Player " << m_gameState.winner() << " has won!" << std::endl;
            return;
        }

        // 2. Basic Validation
        if (gridPos.x == -1 || gridPos.y == -1)
            return;

        int playerId = m_gameState.currentPlayer();
        Game::Move move = Game::Move::Pawn(gridPos.x, gridPos.y, playerId);

        if (m_gameState.applyMove(move))
        {
            if (!m_board.loadFromState(m_gameState))
            {
                std::cout << "Error: Failed to sync board state." << std::endl;
                return;
            }

            checkWinCondition(playerId);

            // If successful and no winner yet:
            if (!m_gameState.isGameOver())
            {
                m_hud.update(m_gameState.currentPlayer());
            }
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

        int playerId = m_gameState.currentPlayer();
        Game::Move move = Game::Move::Wall(gridPos.x, gridPos.y, m_currentWallOri, playerId);
        bool success = m_gameState.applyMove(move);

        if (success)
        {
            if (!m_board.loadFromState(m_gameState))
            {
                std::cout << "Error: Failed to sync board state." << std::endl;
                return;
            }
            std::cout << "Wall placed at " << gridPos.x << ", " << gridPos.y << std::endl;
            m_hud.update(m_gameState.currentPlayer());
            m_isPlacingWall = false;
        }
        else
        {
            std::cout << "Invalid Wall Position!" << std::endl;
        }
    }

    void Application::checkWinCondition(int playerId)
    {
        if (m_gameState.winner() == playerId)
        {
            std::cout << "=== Player " << playerId << " has WON! ===" << std::endl;
        }
    }
}

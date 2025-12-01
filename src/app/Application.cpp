#include "app/Application.hpp"
#include <iostream>

namespace App
{

    Application::Application()
        : m_window(sf::VideoMode({1280, 720}), "Quoridor Isometric")
    {
        m_window.setFramerateLimit(60);
        m_board.init();

        if (!m_renderer.init())
        {
            std::cerr << "Failed to load assets. Exiting.\n";
            exit(-1);
        }

        // Init HUD (Fonts)
        if (!m_hud.init()) {
            std::cerr << "Warning: HUD failed to init (Font missing?)\n";
        }

        // Place Players for Visualization
        // Player 1 at Top (4, 0)
        m_board.getField(4, 8).setOccupantId(1);
        
        // Player 2 at Bottom (4, 8)
        m_board.getField(4, 0).setOccupantId(2);

        m_renderer.handleResize(m_window, m_window.getSize());
    }

    void Application::processEvents() {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                m_renderer.handleResize(m_window, resized->size);
            }
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) m_window.close();
            }
            
            // --- MOUSE MOVED (Hover Effect) ---
            else if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2i gridPos = m_renderer.getMouseGridPos(m_window, mouseMove->position);
                m_renderer.setHoveredTile(gridPos);
            }

            // --- MOUSE CLICK (Movement) ---
            else if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    sf::Vector2i gridPos = m_renderer.getMouseGridPos(m_window, mouseBtn->position);
                    attemptMove(gridPos);
                }
            }
        }
    }

    void Application::attemptMove(sf::Vector2i gridPos) {
        // 1. Basic Validation: Is click inside board?
        if (gridPos.x == -1 || gridPos.y == -1) return;

        // 2. Get the target field
        Game::Field& target = m_board.getField(gridPos.x, gridPos.y);
        
        // 3. Simple Rule: Target must be empty
        if (target.occupantId() != 0) return;

        // 4. Find Current Player Position to clear it
        Game::Field* currentField = nullptr;
        for (const auto& f : m_board.getAllFields()) {
            if (f.occupantId() == m_currentPlayer) {
                currentField = &m_board.getField(f.x(), f.y());
                break;
            }
        }

        if (currentField) {
            // Move Pawn
            currentField->setOccupantId(0);
            target.setOccupantId(m_currentPlayer);

            // Switch Turn
            m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
            m_hud.update(m_currentPlayer);
        }
    }

    void Application::render() {
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
}
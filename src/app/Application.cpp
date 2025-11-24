#include "app/Application.hpp"
#include <iostream>

namespace App {

    Application::Application() 
        // SFML 3 Change: Use braces {} for Vector2 parameters (VideoMode)
        : m_window(sf::VideoMode({800, 600}), "Quoridor POC - SFML 3.0")
    {
        // Setup a simple green circle to prove rendering works
        m_testShape.setRadius(50.f);
        m_testShape.setFillColor(sf::Color::Green);
        
        // SFML 3 Change: setPosition takes a Vector2f, use braces {}
        m_testShape.setPosition({350.f, 250.f});

        // Limit framerate
        m_window.setFramerateLimit(60);
    }

    void Application::run() {
        while (m_window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

    void Application::processEvents() {
        // SFML 3 Breaking Change: pollEvent returns std::optional<sf::Event>
        while (const std::optional event = m_window.pollEvent()) {
            
            // SFML 3 Breaking Change: Use event->is<Type>() or event->getIf<Type>()
            
            // 1. Check for Close
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            }
            // 2. Check for Key Press (Example of getIf)
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    m_window.close();
                }
            }
        }
    }

    void Application::update() {
        // Game logic updates go here
        // For POC, let's just pulse the color slightly
        static float time = 0.0f;
        time += 0.1f;
        // Minimal logic just to show update loop is running
    }

    void Application::render() {
        m_window.clear(sf::Color::Black);
        m_window.draw(m_testShape);
        m_window.display();
    }

}
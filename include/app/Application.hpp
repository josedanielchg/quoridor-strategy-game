#pragma once
#include <SFML/Graphics.hpp>

namespace App {

    class Application {
    public:
        Application();
        void run();

    private:
        void processEvents();
        void update();
        void render();

    private:
        // SFML 3: Window is managed usually the same, but initialization differs slightly
        sf::RenderWindow m_window;
        sf::CircleShape m_testShape;
    };

}
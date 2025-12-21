#pragma once
#include <SFML/Graphics.hpp>
#include "game/Board.hpp"
#include "ui/GameRenderer.hpp"
#include "ui/Hud.hpp"

namespace App {

    class Application {
    public:
        Application();
        void run();

    private:
        void processEvents();
        void update();
        void render();
        void attemptMove(sf::Vector2i gridPos);

        void attemptPlaceWall(sf::Vector2i gridPos);
        void toggleWallMode();
        void rotateWall();
        void checkWinCondition();

    private:
        // SFML 3: Window is managed usually the same, but initialization differs slightly
        sf::RenderWindow m_window;
        sf::CircleShape m_testShape;
        Game::Board m_board;
        UI::GameRenderer m_renderer;
        UI::Hud m_hud;

        int m_currentPlayer = 1;
        int m_winner = 0;  // 0 = no winner, 1 or 2 = winner

        bool m_isPlacingWall = false;
        Game::Orientation m_currentWallOri = Game::Orientation::Horizontal;
    };

}
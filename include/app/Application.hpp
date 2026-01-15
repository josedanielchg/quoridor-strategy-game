#pragma once
#include <SFML/Graphics.hpp>
#include "game/Board.hpp"
#include "game/GameState.hpp"
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
        void checkWinCondition(int playerId);

    private:
        sf::RenderWindow m_window;
        sf::CircleShape m_testShape;
        Game::Board m_board;
        Game::GameState m_gameState;
        UI::GameRenderer m_renderer;
        UI::Hud m_hud;


        bool m_isPlacingWall = false;
        Game::Orientation m_currentWallOri = Game::Orientation::Horizontal;
    };

}

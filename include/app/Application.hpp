#pragma once
#include <SFML/Graphics.hpp>
<<<<<<< Updated upstream
#include <memory>
#include "app/GameScreen.hpp"
#include "app/Screen.hpp"
#include "app/TitleScreen.hpp"
=======
#include "ai/AiEngine.hpp"
#include "game/Board.hpp"
#include "game/GameState.hpp"
#include "ui/GameRenderer.hpp"
#include "ui/Hud.hpp"
#include "ui/PauseMenu.hpp"
>>>>>>> Stashed changes

namespace App
{

    class Application
    {
    public:
        Application();
        void run();

    private:
        void processEvents();
        void update();
        void render();
<<<<<<< Updated upstream
        void setCurrentScreen(Screen *screen);

    private:
        sf::RenderWindow m_window;
        sf::Clock m_clock;
        std::unique_ptr<TitleScreen> m_titleScreen;
        std::unique_ptr<GameScreen> m_gameScreen;
        Screen *m_currentScreen = nullptr;
=======
        void attemptMove(sf::Vector2i gridPos);
        void resetGame();
        void togglePauseMenu();

        void attemptPlaceWall(sf::Vector2i gridPos);
        void toggleWallMode();
        void rotateWall();
        void checkWinCondition(int playerId);
        void runAiTurn();

    private:
        sf::RenderWindow m_window;
        sf::CircleShape m_testShape;
        Game::Board m_board;
        Game::GameState m_gameState;
        UI::GameRenderer m_renderer;
        UI::Hud m_hud;
        UI::PauseMenu m_pauseMenu;
        Game::AiEngine m_aiEngine;


        bool m_isPlacingWall = false;
        Game::Orientation m_currentWallOri = Game::Orientation::Horizontal;
>>>>>>> Stashed changes
    };

}

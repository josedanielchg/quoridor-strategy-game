#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "heuristic/HeuristicEngine.hpp"
#include "app/Screen.hpp"
#include "game/Board.hpp"
#include "game/GameState.hpp"
#include "ui/GameRenderer.hpp"
#include "ui/Hud.hpp"
#include "ui/PauseMenu.hpp"

namespace App
{
    class GameScreen : public Screen
    {
    public:
        bool init() override;
        void setOnQuit(std::function<void()> onQuit);

        void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
        void handleResize(sf::RenderWindow &window, sf::Vector2u size) override;
        void update(float dt) override;
        void render(sf::RenderWindow &window) override;

    private:
        void attemptMove(sf::Vector2i gridPos);
        void resetGame();
        void togglePauseMenu();
        void attemptPlaceWall(sf::Vector2i gridPos);
        void toggleWallMode();
        void rotateWall();
        void checkWinCondition(int playerId);
        void runHeuristicTurn();

        Game::Board m_board;
        Game::GameState m_gameState;
        UI::GameRenderer m_renderer;
        UI::Hud m_hud;
        UI::PauseMenu m_pauseMenu;
        Game::HeuristicEngine m_heuristicEngine;
        std::function<void()> m_onQuit;

        bool m_isPlacingWall = false;
        Game::Orientation m_currentWallOri = Game::Orientation::Horizontal;
    };
}

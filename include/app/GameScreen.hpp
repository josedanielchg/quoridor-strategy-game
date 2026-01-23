#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <functional>
#include <future>
#include "heuristic/HeuristicEngine.hpp"
#include "app/Screen.hpp"
#include "game/Board.hpp"
#include "game/GameState.hpp"
#include "ui/GameRenderer.hpp"
#include "ui/Hud.hpp"
#include "ui/PauseMenu.hpp"
#include "ui/WinnerMenu.hpp"

namespace App
{
    class GameScreen : public Screen
    {
    public:
        static constexpr const char *MUSIC_PATH = "assets/sound/game_theme.mp3";

        bool init() override;
        void setOnQuit(std::function<void()> onQuit);

        void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
        void handleResize(sf::RenderWindow &window, sf::Vector2u size) override;
        void update(float dt) override;
        void render(sf::RenderWindow &window) override;

    private:
        const char *getMusicPath() const override { return MUSIC_PATH; }
        void attemptMove(sf::Vector2i gridPos);
        void resetGame();
        void togglePauseMenu();
        void attemptPlaceWall(sf::Vector2i gridPos);
        void toggleWallMode();
        void rotateWall();
        void checkWinCondition(int playerId);
        void runHeuristicTurn();
        void updateHeuristicTurn();

        Game::Board m_board;
        Game::GameState m_gameState;
        UI::GameRenderer m_renderer;
        UI::Hud m_hud;
        UI::PauseMenu m_pauseMenu;
        Game::HeuristicEngine m_heuristicEngine;
        UI::WinnerMenu m_winnerMenu;
        std::function<void()> m_onQuit;

        bool m_isPlacingWall = false;
        Game::Orientation m_currentWallOri = Game::Orientation::Horizontal;
        bool m_cpuPending = false;
        bool m_cpuThinking = false;
        uint64_t m_stateVersion = 0;
        uint64_t m_cpuStateVersion = 0;
        std::future<Game::Move> m_cpuFuture;
    };
}

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
#include "ui/InGameBottomBar.hpp"
#include "ui/PauseMenu.hpp"
#include "ui/WinnerMenu.hpp"

namespace App
{
    class GameScreen : public Screen
    {
    public:
        static constexpr const char *MUSIC_PATH = "assets/sound/game_theme.mp3";

        enum class GameMode
        {
            SinglePlayer,
            Multiplayer
        };

        bool init() override; // Init gameplay and UI #
        void setGameMode(GameMode mode); // Set game mode #
        void setOnQuit(std::function<void()> onQuit); // Register quit callback #

        void handleEvent(const sf::Event &event, sf::RenderWindow &window) override; // Handle input #
        void handleResize(sf::RenderWindow &window, sf::Vector2u size) override; // Reflow views #
        void update(float dt) override; // Update game loop #
        void render(sf::RenderWindow &window) override; // Draw gameplay #
        void onEnter() override; // Enter screen #
        void onExit() override; // Exit screen #

    private:
        const char *getMusicPath() const override { return MUSIC_PATH; } // Music path #
        void attemptMove(sf::Vector2i gridPos); // Try pawn move #
        void resetGame(); // Reset gameplay state #
        // Toggle pause menu visibility.
        void togglePauseMenu(); // Toggle pause menu #
        void attemptPlaceWall(sf::Vector2i gridPos); // Try wall placement #
        void toggleWallMode(); // Toggle wall mode #
        void rotateWall(); // Rotate wall orientation #
        void checkWinCondition(int playerId); // Check winner #
        void runHeuristicTurn(); // Schedule CPU turn #
        void resetUIState(); // Reset UI flags #
        void updateHeuristicTurn(); // Update CPU turn #

        Game::Board m_board;
        Game::GameState m_gameState;
        UI::GameRenderer m_renderer;
        UI::Hud m_hud;
        UI::InGameBottomBar m_bottomBar;
        UI::PauseMenu m_pauseMenu;
        Game::HeuristicEngine m_heuristicEngine;
        UI::WinnerMenu m_winnerMenu;
        std::function<void()> m_onQuit;

        GameMode m_gameMode = GameMode::SinglePlayer;
        bool m_isPlacingWall = false;
        Game::Orientation m_currentWallOri = Game::Orientation::Horizontal;
        sf::Vector2i m_lastHoverGrid{-2, -2};
        bool m_winSfxPlayed = false;
        bool m_cpuPending = false;
        bool m_cpuThinking = false;
        uint64_t m_stateVersion = 0;
        uint64_t m_cpuStateVersion = 0;
        std::future<Game::Move> m_cpuFuture;
    };
}

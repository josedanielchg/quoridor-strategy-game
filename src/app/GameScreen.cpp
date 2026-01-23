#include "app/GameScreen.hpp"
#include "game/Move.hpp"
#include "game/GameRules.hpp"
#include <chrono>
#include <future>
#include <iostream>
#include <utility>

namespace App
{
    bool GameScreen::init()
    {
        m_board.init();
        Game::initGameState(m_gameState);
        m_board.loadFromState(m_gameState);
        m_stateVersion = 0;
        m_cpuPending = false;
        m_cpuThinking = false;

        if (!m_renderer.init())
        {
            std::cerr << "Failed to init renderer\n";
            return false;
        }
        if (!m_hud.init())
            std::cerr << "Warning: HUD failed\n";
        else
            m_hud.update(Game::currentPlayer(m_gameState),
                         m_gameState.wallsRemaining[0],
                         m_gameState.wallsRemaining[1],
                         Game::GameState::MAX_WALLS_PER_PLAYER);

        if (!m_bottomBar.init())
            std::cerr << "Warning: Bottom bar failed\n";
        m_bottomBar.setWallPlacementActive(m_isPlacingWall);

        if (!m_pauseMenu.init())
        {
            std::cerr << "Warning: Pause menu failed\n";
        }
        else
        {
            m_pauseMenu.setOnResume([this]() { m_pauseMenu.setEnabled(false); });
            m_pauseMenu.setOnRestart([this]()
                                     {
                                         resetGame();
                                         m_pauseMenu.setEnabled(false);
                                     });
            m_pauseMenu.setOnQuit([this]()
                                  {
                                      resetUIState();
                                      if (m_onQuit)
                                          m_onQuit();
                                  });
        }

        if (!m_winnerMenu.init())
        {
            std::cerr << "Warning: Winner menu failed\n";
        }
        else
        {
            m_winnerMenu.setOnRestart([this]()
                                      {
                                          resetGame();
                                          m_winnerMenu.setEnabled(false);
                                      });
            m_winnerMenu.setOnQuit([this]()
                                   {
                                       resetUIState();
                                       if (m_onQuit)
                                           m_onQuit();
                                   });
        }

        m_bottomBar.setOnClick([this]()
                               {
                                   if (!m_pauseMenu.isEnabled() && !m_winnerMenu.isEnabled())
                                       togglePauseMenu();
                               });
        m_bottomBar.setOnToggleWallMode([this]() { toggleWallMode(); });
        m_bottomBar.setOnRotateWall([this]() { rotateWall(); });

        return true;
    }

    void GameScreen::setGameMode(GameMode mode)
    {
        m_gameMode = mode;
        resetGame();
    }

    void GameScreen::setOnQuit(std::function<void()> onQuit)
    {
        m_onQuit = std::move(onQuit);
    }

    void GameScreen::onEnter()
    {
        Screen::onEnter();
        resetUIState();
    }

    void GameScreen::onExit()
    {
        resetUIState();
        Screen::onExit();
    }

    void GameScreen::handleEvent(const sf::Event &event, sf::RenderWindow &window)
    {
        if (event.is<sf::Event::Resized>())
            return;

        if (m_winnerMenu.isEnabled())
        {
            if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>())
            {
                m_winnerMenu.updateHover(window, mouseMove->position);
            }
            else if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseBtn->button == sf::Mouse::Button::Left)
                    m_winnerMenu.handleClick(window, mouseBtn->position);
            }
            return;
        }

        if (!m_pauseMenu.isEnabled())
        {
            if (m_bottomBar.handleEvent(event, window))
                return;
        }

        if (const auto *key = event.getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scancode::Escape)
            {
                togglePauseMenu();
                return;
            }
            if (m_pauseMenu.isEnabled())
                return;
            if (key->scancode == sf::Keyboard::Scancode::W)
                toggleWallMode();
            if (key->scancode == sf::Keyboard::Scancode::R)
                rotateWall();
        }
        else if (m_pauseMenu.isEnabled())
        {
            if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>())
            {
                m_pauseMenu.updateHover(window, mouseMove->position);
            }
            else if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseBtn->button == sf::Mouse::Button::Left)
                    m_pauseMenu.handleClick(window, mouseBtn->position);
            }
        }
        else if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>())
        {
            sf::Vector2i gridPos = m_renderer.getMouseGridPos(window, mouseMove->position);

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
        else if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseBtn->button == sf::Mouse::Button::Left)
            {
                sf::Vector2i gridPos = m_renderer.getMouseGridPos(window, mouseBtn->position);

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

    void GameScreen::handleResize(sf::RenderWindow &window, sf::Vector2u size)
    {
        m_renderer.handleResize(window, size);
    }

    void GameScreen::update(float /*dt*/)
    {
        updateHeuristicTurn();
    }

    void GameScreen::render(sf::RenderWindow &window)
    {
        m_renderer.render(window, m_board);
        m_bottomBar.render(window);
        m_hud.render(window);
        m_pauseMenu.render(window);
        m_winnerMenu.render(window);
    }

    void GameScreen::attemptMove(sf::Vector2i gridPos)
    {
        if (Game::isGameOver(m_gameState))
        {
            std::cout << "Game Over! Player " << Game::winner(m_gameState) << " has won!" << std::endl;
            return;
        }

        if (gridPos.x == -1 || gridPos.y == -1)
            return;

        int playerId = Game::currentPlayer(m_gameState);
        Game::Move move = Game::Move::Pawn(gridPos.x, gridPos.y, playerId);

        if (Game::applyMove(m_gameState, move))
        {
            ++m_stateVersion;
            if (!m_board.loadFromState(m_gameState))
            {
                std::cout << "Error: Failed to sync board state." << std::endl;
                return;
            }

            checkWinCondition(playerId);

            if (!Game::isGameOver(m_gameState))
            {
                m_hud.update(Game::currentPlayer(m_gameState),
                            m_gameState.wallsRemaining[0],
                            m_gameState.wallsRemaining[1],
                            Game::GameState::MAX_WALLS_PER_PLAYER);
            }

            runHeuristicTurn();
        }
        else
        {
            std::cout << "Invalid Move!" << std::endl;
        }
    }

    void GameScreen::toggleWallMode()
    {
        m_isPlacingWall = !m_isPlacingWall;
        m_bottomBar.setWallPlacementActive(m_isPlacingWall);
        std::cout << "Wall Mode: " << (m_isPlacingWall ? "ON" : "OFF") << std::endl;
    }

    void GameScreen::rotateWall()
    {
        if (m_currentWallOri == Game::Orientation::Horizontal)
            m_currentWallOri = Game::Orientation::Vertical;
        else
            m_currentWallOri = Game::Orientation::Horizontal;
    }

    void GameScreen::attemptPlaceWall(sf::Vector2i gridPos)
    {
        if (gridPos.x == -1)
            return;

        int playerId = Game::currentPlayer(m_gameState);
        Game::Move move = Game::Move::Wall(gridPos.x, gridPos.y, m_currentWallOri, playerId);
        bool success = Game::applyMove(m_gameState, move);

        if (success)
        {
            ++m_stateVersion;
            if (!m_board.loadFromState(m_gameState))
            {
                std::cout << "Error: Failed to sync board state." << std::endl;
                return;
            }
            std::cout << "Wall placed at " << gridPos.x << ", " << gridPos.y << std::endl;
            m_hud.update(Game::currentPlayer(m_gameState),
                        m_gameState.wallsRemaining[0],
                        m_gameState.wallsRemaining[1],
                        Game::GameState::MAX_WALLS_PER_PLAYER);
            m_isPlacingWall = false;
            m_bottomBar.setWallPlacementActive(m_isPlacingWall);

            runHeuristicTurn();
        }
        else
        {
            std::cout << "Invalid Wall Position!" << std::endl;
        }
    }

    void GameScreen::runHeuristicTurn()
    {
        if (m_gameMode != GameMode::SinglePlayer)
            return;
        if (Game::isGameOver(m_gameState))
            return;
        if (Game::currentPlayer(m_gameState) != 2)
            return;
        if (m_cpuThinking)
            return;

        m_cpuPending = true;
    }

    void GameScreen::updateHeuristicTurn()
    {
        if (m_cpuPending && !m_cpuThinking)
        {
            if (Game::isGameOver(m_gameState) || Game::currentPlayer(m_gameState) != 2)
            {
                m_cpuPending = false;
                return;
            }

            m_cpuPending = false;
            m_cpuThinking = true;
            m_cpuStateVersion = m_stateVersion;
            Game::GameState snapshot = m_gameState;
            m_cpuFuture = std::async(std::launch::async, [this, snapshot]() mutable
                                     { return m_heuristicEngine.findBestMove(snapshot); });
        }

        if (!m_cpuThinking)
            return;

        if (!m_cpuFuture.valid())
        {
            m_cpuThinking = false;
            return;
        }

        if (m_cpuFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
            return;

        Game::Move move = m_cpuFuture.get();
        m_cpuThinking = false;

        if (m_stateVersion != m_cpuStateVersion)
            return;
        if (Game::isGameOver(m_gameState) || Game::currentPlayer(m_gameState) != 2)
            return;

        if (!Game::applyMove(m_gameState, move))
        {
            std::cout << "Heuristic move failed.\n";
            return;
        }
        ++m_stateVersion;

        if (!m_board.loadFromState(m_gameState))
        {
            std::cout << "Error: Failed to sync board state." << std::endl;
            return;
        }

        checkWinCondition(2);

        if (!Game::isGameOver(m_gameState))
        {
            m_hud.update(Game::currentPlayer(m_gameState),
                         m_gameState.wallsRemaining[0],
                         m_gameState.wallsRemaining[1],
                         Game::GameState::MAX_WALLS_PER_PLAYER);
        }
    }

    void GameScreen::checkWinCondition(int playerId)
    {
        if (Game::winner(m_gameState) == playerId)
        {
            std::cout << "=== Player " << playerId << " has WON! ===" << std::endl;
            m_winnerMenu.setWinner(playerId);
            m_winnerMenu.setEnabled(true);
            m_pauseMenu.setEnabled(false);
            m_isPlacingWall = false;
            m_bottomBar.setWallPlacementActive(m_isPlacingWall);
        }
    }

    void GameScreen::resetGame()
    {
        m_board.init();
        Game::initGameState(m_gameState);
        m_board.loadFromState(m_gameState);
        resetUIState();
        m_stateVersion = 0;
        m_cpuPending = false;
        m_cpuThinking = false;

        m_isPlacingWall = false;
        m_currentWallOri = Game::Orientation::Horizontal;
        m_winnerMenu.setEnabled(false);
        restartMusic();

        m_hud.update(Game::currentPlayer(m_gameState),
                    m_gameState.wallsRemaining[0],
                    m_gameState.wallsRemaining[1],
                    Game::GameState::MAX_WALLS_PER_PLAYER);
    }

    void GameScreen::togglePauseMenu()
    {
        const bool wasEnabled = m_pauseMenu.isEnabled();
        m_pauseMenu.toggleEnabled();

        if (!wasEnabled)
        {
            m_renderer.setHoveredTile({-1, -1});
            m_renderer.setWallPreview(false, {0, 0}, m_currentWallOri);
        }
    }

    void GameScreen::resetUIState()
    {
        m_pauseMenu.setEnabled(false);
        m_winnerMenu.setEnabled(false);
        m_isPlacingWall = false;
        m_currentWallOri = Game::Orientation::Horizontal;
        m_renderer.setHoveredTile({-1, -1});
        m_renderer.setWallPreview(false, {0, 0}, m_currentWallOri);
        m_bottomBar.resetHover();
        m_bottomBar.setWallPlacementActive(false);
    }
}

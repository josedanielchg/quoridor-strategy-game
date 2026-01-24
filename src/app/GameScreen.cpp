#include "app/GameScreen.hpp"
#include "game/Move.hpp"
#include "game/GameRules.hpp"
#include "audio/SfxManager.hpp"
#include <chrono>
#include <future>
#include <exception>
#include <stdexcept>
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

        try
        {
            if (!m_renderer.init())
            {
                throw std::runtime_error("GameRenderer initialization failed");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }

        bool hudOk = false;
        try
        {
            if (!m_hud.init())
                throw std::runtime_error("Hud initialization failed");
            hudOk = true;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
        }
        if (hudOk)
        {
            m_hud.update(Game::currentPlayer(m_gameState),
                         m_gameState.wallsRemaining[0],
                         m_gameState.wallsRemaining[1],
                         Game::GameState::MAX_WALLS_PER_PLAYER);
        }

        try
        {
            if (!m_bottomBar.init())
                throw std::runtime_error("InGameBottomBar initialization failed");
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
        }
        m_bottomBar.setWallPlacementActive(m_isPlacingWall);

        bool pauseOk = false;
        try
        {
            if (!m_pauseMenu.init())
                throw std::runtime_error("PauseMenu initialization failed");
            pauseOk = true;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
        }

        if (pauseOk)
        {
            m_pauseMenu.setOnResume([this]()
                                    { m_pauseMenu.setEnabled(false); });
            m_pauseMenu.setOnRestart([this]()
                                     {
                                         resetGame();
                                         m_pauseMenu.setEnabled(false); });
            m_pauseMenu.setOnQuit([this]()
                                  {
                                      resetUIState();
                                      if (m_onQuit)
                                          m_onQuit(); });
        }

        bool winnerOk = false;
        try
        {
            if (!m_winnerMenu.init())
                throw std::runtime_error("WinnerMenu initialization failed");
            winnerOk = true;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\n";
        }

        if (winnerOk)
        {
            m_winnerMenu.setOnRestart([this]()
                                      {
                                          resetGame();
                                          m_winnerMenu.setEnabled(false); });
            m_winnerMenu.setOnQuit([this]()
                                   {
                                       resetUIState();
                                       if (m_onQuit)
                                           m_onQuit(); });
        }

        // Force static wall preview sprites to initialize during startup (fail fast).
        (void)Game::Wall::previewUpperSprite();
        (void)Game::Wall::previewDownSprite();

        m_bottomBar.setOnClick([this]()
                               {
                                   if (!m_pauseMenu.isEnabled() && !m_winnerMenu.isEnabled())
                                       togglePauseMenu(); });
        m_bottomBar.setOnToggleWallMode([this]()
                                        { toggleWallMode(); });
        m_bottomBar.setOnRotateWall([this]()
                                    { rotateWall(); });

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
            if (gridPos.x >= 0 && gridPos.y >= 0)
            {
                if (gridPos != m_lastHoverGrid)
                {
                    Audio::SfxManager::instance().play(Audio::SfxId::Hover);
                    m_lastHoverGrid = gridPos;
                }
            }
            else
            {
                m_lastHoverGrid = {-2, -2};
            }

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
            Audio::SfxManager::instance().play(Audio::SfxId::Click);
            Audio::SfxManager::instance().play(Audio::SfxId::Move);
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
            Audio::SfxManager::instance().play(Audio::SfxId::Click);
            Audio::SfxManager::instance().play(Audio::SfxId::Wall);
            ++m_stateVersion;
            if (!m_board.loadFromState(m_gameState))
            {
                std::cout << "Error: Failed to sync board state." << std::endl;
                return;
            }
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
        if (move.type() == Game::MoveType::PawnMove)
            Audio::SfxManager::instance().play(Audio::SfxId::Move);
        else
            Audio::SfxManager::instance().play(Audio::SfxId::Wall);
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
            if (!m_winSfxPlayed)
            {
                Audio::SfxManager::instance().play(Audio::SfxId::Win);
                m_winSfxPlayed = true;
            }
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
            Audio::SfxManager::instance().play(Audio::SfxId::MenuOpen);
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
        m_lastHoverGrid = {-2, -2};
        m_winSfxPlayed = false;
        m_renderer.setHoveredTile({-1, -1});
        m_renderer.setWallPreview(false, {0, 0}, m_currentWallOri);
        m_bottomBar.resetHover();
        m_bottomBar.setWallPlacementActive(false);
    }
}

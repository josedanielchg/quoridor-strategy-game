#include "app/GameScreen.hpp"
#include "game/Move.hpp"
#include "game/GameRules.hpp"
#include <iostream>
#include <utility>

namespace App
{
    bool GameScreen::init()
    {
        m_board.init();
        Game::initGameState(m_gameState);
        m_board.loadFromState(m_gameState);

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
                                       if (m_onQuit)
                                           m_onQuit();
                                   });
        }

        m_winnerMenu.setEnabled(true);

        return true;
    }

    void GameScreen::setOnQuit(std::function<void()> onQuit)
    {
        m_onQuit = std::move(onQuit);
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
    }

    void GameScreen::render(sf::RenderWindow &window)
    {
        m_renderer.render(window, m_board);
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
        }
        else
        {
            std::cout << "Invalid Move!" << std::endl;
        }
    }

    void GameScreen::toggleWallMode()
    {
        m_isPlacingWall = !m_isPlacingWall;
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
        }
        else
        {
            std::cout << "Invalid Wall Position!" << std::endl;
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
        }
    }

    void GameScreen::resetGame()
    {
        m_board.init();
        Game::initGameState(m_gameState);
        m_board.loadFromState(m_gameState);

        m_isPlacingWall = false;
        m_currentWallOri = Game::Orientation::Horizontal;
        m_winnerMenu.setEnabled(false);

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
}

#pragma once
#include "game/Field.hpp"
#include "game/Wall.hpp"
#include "game/Pawn.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <optional>

namespace Game
{
    struct GameState;

    class Board
    {
    public:
        static constexpr int SIZE = 9;

    private:
        std::vector<Field> m_fields;
        std::vector<Wall> m_walls;
        std::vector<Pawn> m_pawns;
        sf::Texture m_backgroundTexture;
        sf::Sprite m_backgroundSprite;
        bool m_hasBackground = false;

    public:
        Board(); // Initialize board data #

        void init(bool loadVisuals = true); // Reset grid and visuals #
        void drawBackground(sf::RenderWindow &window) const; // Draw board background #

        Field &getField(int x, int y); // Access a mutable field #
        const Field &getField(int x, int y) const; // Access a field #
        const std::vector<Field> &getAllFields() const; // Get all fields #

        const std::vector<Wall> &getAllWalls() const; // Get all walls #
        const std::vector<Pawn> &getAllPawns() const; // Get all pawns #
        void setPawns(int p1x, int p1y, int p2x, int p2y); // Set pawn positions #
        bool loadFromState(const GameState &state); // Sync board from state #

        const Pawn *getPawnAt(int x, int y) const; // Find pawn at grid #
        Pawn *getPawnById(int id); // Find pawn by id #
        const Pawn *getPawnById(int id) const; // Find pawn by id #

        bool placeWall(int x, int y, Orientation orientation); // Place a wall #
        bool movePawn(int pawnId, int targetX, int targetY); // Move a pawn #

        bool isValid(int x, int y) const; // Check bounds #

        void toggleWall(int x, int y, Orientation ori, bool blocking); // Block/unblock edges #
    };

}

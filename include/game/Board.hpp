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
        Board();

        // Initialize grid and neighbor connections
        void init(bool loadVisuals = true);
        void drawBackground(sf::RenderWindow &window) const;

        // Graph Operations
        Field &getField(int x, int y);
        const Field &getField(int x, int y) const;
        const std::vector<Field> &getAllFields() const;

        // Entity Access
        const std::vector<Wall> &getAllWalls() const;
        const std::vector<Pawn> &getAllPawns() const;
        void setPawns(int p1x, int p1y, int p2x, int p2y);
        bool loadFromState(const GameState &state);

        // Helper: Returns pointer to pawn at x,y or nullptr
        const Pawn *getPawnAt(int x, int y) const;
        Pawn *getPawnById(int id);
        const Pawn *getPawnById(int id) const;

        // Actions
        bool placeWall(int x, int y, Orientation orientation);
        bool movePawn(int pawnId, int targetX, int targetY);

        bool isValid(int x, int y) const;

        void toggleWall(int x, int y, Orientation ori, bool blocking);
    };

}

#pragma once
#include <SFML/System/Vector2.hpp>

namespace UI
{
    inline constexpr sf::Vector2f UI_DESIGN_SIZE{1728.f, 1117.f};
    inline constexpr sf::Vector2f FIELD_SIZE{221.f, 156.f};
    inline constexpr sf::Vector2f PAWN_P1_SIZE{120.f, 175.f};
    inline constexpr sf::Vector2f PAWN_P2_SIZE{109.f, 180.f};
    inline constexpr sf::Vector2f WALL_SIZE{121.f, 135.f};
    inline constexpr float PAWN_BASELINE_OFFSET = 10.f;
    inline constexpr float BOARD_SCALE = 0.78f;
}

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
    inline
     constexpr sf::Vector2f WIN_BANNER_SIZE{685.f, 843.f};
    inline constexpr float WIN_BANNER_BUTTON_GAP = -8.f;
    inline constexpr float WIN_BANNER_BUTTON_SPAN = 537.f;
    inline constexpr float INGAME_BOTTOM_BAR_HEIGHT = 96.f;
    inline constexpr sf::Vector2f INGAME_MENU_ICON_SIZE{56.f, 56.f};
    inline constexpr float INGAME_MENU_ICON_PADDING = 24.f;

    inline constexpr float HOWTO_LAYOUT_REF_WIDTH = 1920.f;
    inline constexpr float HOWTO_LAYOUT_REF_HEIGHT = 1200.f;
    inline constexpr float HOWTO_BOARD_LEFT_RATIO = 326.f / HOWTO_LAYOUT_REF_WIDTH;
    inline constexpr float HOWTO_BOARD_TOP_RATIO = 210.f / HOWTO_LAYOUT_REF_HEIGHT;
    inline constexpr float HOWTO_BOARD_WIDTH_RATIO = (1830.f - 326.f) / HOWTO_LAYOUT_REF_WIDTH;
    inline constexpr float HOWTO_BOARD_HEIGHT_RATIO = (725.f - 210.f) / HOWTO_LAYOUT_REF_HEIGHT;
    inline constexpr float HOWTO_DIALOGUE_LEFT_RATIO = 60.f / HOWTO_LAYOUT_REF_WIDTH;
    inline constexpr float HOWTO_DIALOGUE_TOP_RATIO = 990.f / HOWTO_LAYOUT_REF_HEIGHT;
    inline constexpr float HOWTO_DIALOGUE_WIDTH_RATIO = (1870.f - 60.f) / HOWTO_LAYOUT_REF_WIDTH;
    inline constexpr float HOWTO_DIALOGUE_HEIGHT_RATIO = (1190.f - 990.f) / HOWTO_LAYOUT_REF_HEIGHT;
    inline constexpr float HOWTO_NAMEPLATE_LEFT_RATIO = 53.f / HOWTO_LAYOUT_REF_WIDTH;
    inline constexpr float HOWTO_NAMEPLATE_TOP_RATIO = 960.f / HOWTO_LAYOUT_REF_HEIGHT;
    inline constexpr float HOWTO_NAMEPLATE_WIDTH_RATIO = 191.f / HOWTO_LAYOUT_REF_WIDTH;
    inline constexpr float HOWTO_NAMEPLATE_HEIGHT_RATIO = 28.f / HOWTO_LAYOUT_REF_HEIGHT;
}

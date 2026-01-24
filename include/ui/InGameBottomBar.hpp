#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

namespace UI
{
    class InGameBottomBar
    {
    public:
        InGameBottomBar(); // Build bar state #
        bool init(); // Load icons and font #
        void setOnClick(std::function<void()> onClick); // Register menu click #
        void setOnToggleWallMode(std::function<void()> onToggleWallMode); // Register wall mode toggle #
        void setOnRotateWall(std::function<void()> onRotateWall); // Register rotate action #
        void setWallPlacementActive(bool active); // Set wall mode state #
        bool handleEvent(const sf::Event &event, sf::RenderWindow &window); // Handle input #
        void render(sf::RenderWindow &window); // Draw bottom bar #
        void resetHover(); // Clear hover state #

    private:
        void updateLayout(const sf::RenderWindow &window); // Recompute layout #
        void updateHoverState(const sf::Vector2f &worldPos); // Update hover flags #
        void applyHoverColor(); // Apply hover colors #
        void layoutControlsIcons(); // Position control icons/labels #
        void updateWallIndicatorColors(); // Refresh wall mode visuals #

        static constexpr float CONTROL_ICON_W_RATIO = (122.f / 1728.f) * 0.5f;
        static constexpr float CONTROL_ICON_H_RATIO = (118.f / 1117.f) * 0.5f;
        static constexpr float CONTROL_ICON_GAP_PX = 48.f;
        static constexpr float CONTROL_LABEL_GAP_PX = 12.f;
        static constexpr float CONTROL_LABEL_FONT_SIZE = 26.f;

        sf::View m_view;
        sf::RectangleShape m_bar;
        sf::Texture m_hamburgerTexture;
        sf::Sprite m_hamburgerSprite;
        sf::FloatRect m_barBounds;
        sf::FloatRect m_hamburgerBounds;
        bool m_hovered = false;
        bool m_wHovered = false;
        bool m_rHovered = false;
        std::function<void()> m_onClick;
        std::function<void()> m_onToggleWallMode;
        std::function<void()> m_onRotateWall;

        sf::Texture m_wIconTexture;
        sf::Texture m_rIconTexture;
        sf::Sprite m_wIconSprite;
        sf::Sprite m_rIconSprite;
        sf::FloatRect m_wIconBounds;
        sf::FloatRect m_rIconBounds;
        sf::Font m_font;
        sf::Text m_wLabel;
        sf::Text m_rLabel;
        bool m_hasFont = false;
        bool m_wallModeActive = false;
        sf::FloatRect m_wControlBounds;
        sf::FloatRect m_rControlBounds;

        sf::Vector2f m_viewSize;
        sf::Vector2f m_viewTopLeft;
        float m_layoutScale = 1.f;
        float m_barHeight = 0.f;
        float m_barPadding = 0.f;
    };
}

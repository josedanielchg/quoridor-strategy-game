#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "app/Screen.hpp"

namespace App
{
    class CreditsScreen : public Screen
    {
    public:
        static constexpr const char *MUSIC_PATH = "assets/sound/main_theme.mp3";

        CreditsScreen(); // Build screen state #
        bool init() override; // Load assets and text #
        void setOnBack(std::function<void()> onBack); // Register back callback #

        void handleEvent(const sf::Event &event, sf::RenderWindow &window) override; // Handle input #
        void handleResize(sf::RenderWindow &window, sf::Vector2u size) override; // Reflow layout #
        void update(float dt) override; // Advance animation #
        void render(sf::RenderWindow &window) override; // Draw screen #

    private:
        const char *getMusicPath() const override { return MUSIC_PATH; } // Music path #
        void updateLayout(const sf::View &view); // Scale/position elements #
        void layoutText(const sf::Vector2f &center, float textScale); // Place text blocks #
        void updateTextOrigins(sf::Text &text); // Center text origin #

        static constexpr int SPRITE_COLUMNS = 7;
        static constexpr int SPRITE_ROWS = 5;
        static constexpr int LAST_ROW_FRAMES = 3;
        static constexpr int FRAME_WIDTH = 1080;
        static constexpr int FRAME_HEIGHT = 698;
        static constexpr int TOTAL_FRAMES = 31;
        static constexpr float FRAME_TIME = 1.f / 12.f;

        sf::Texture m_backgroundTexture;
        sf::Sprite m_backgroundSprite;
        sf::Font m_font;
        sf::Text m_titleText;
        sf::Text m_bodyText;
        sf::Text m_titleShadow;
        sf::Text m_bodyShadow;
        std::function<void()> m_onBack;
        float m_frameTimer = 0.f;
        int m_frameIndex = 0;
    };
}

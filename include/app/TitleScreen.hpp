#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "app/Screen.hpp"

namespace App
{
    class TitleScreen : public Screen
    {
    public:
        static constexpr const char *MUSIC_PATH = "assets/sound/main_theme.mp3";

        TitleScreen();
        bool init() override;
        void setOnStart(std::function<void()> onStart);

        void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
        void handleResize(sf::RenderWindow &window, sf::Vector2u size) override;
        void update(float dt) override;
        void render(sf::RenderWindow &window) override;

    private:
        const char *getMusicPath() const override { return MUSIC_PATH; }
        void updateLayout(const sf::View &view);

        static constexpr int SPRITE_COLUMNS = 12;
        static constexpr int SPRITE_ROWS = 11;
        static constexpr int LAST_ROW_FRAMES = 6;
        static constexpr int FRAME_WIDTH = 696;
        static constexpr int FRAME_HEIGHT = 450;
        static constexpr float FRAME_TIME = 1.f / 24.f;

        sf::Texture m_backgroundTexture;
        sf::Sprite m_backgroundSprite;
        sf::Font m_font;
        sf::Text m_promptLine1;
        sf::Text m_promptLine2;
        sf::VertexArray m_promptGradient;
        std::function<void()> m_onStart;
        float m_blinkTimer = 0.f;
        float m_frameTimer = 0.f;
        int m_frameIndex = 0;
    };
}

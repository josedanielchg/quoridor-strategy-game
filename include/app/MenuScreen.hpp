#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "app/Screen.hpp"

namespace App
{
    class MenuScreen : public Screen
    {
    public:
        static constexpr const char *MUSIC_PATH = "assets/sound/main_theme.mp3";

        enum class Option
        {
            SinglePlayer,
            Multiplayer,
            HowToPlay,
            Credits
        };

        MenuScreen();
        bool init() override;
        void setOnOptionSelected(std::function<void(Option)> onSelect);

        void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
        void handleResize(sf::RenderWindow &window, sf::Vector2u size) override;
        void update(float dt) override;
        void render(sf::RenderWindow &window) override;

    private:
        const char *getMusicPath() const override { return MUSIC_PATH; }
        void updateLayout(const sf::View &view);
        void setSelectedIndex(std::size_t index);
        void applyOptionColors(std::uint8_t selectedAlpha);
        void selectNext(int direction);
        void activateSelected();

        static constexpr int SPRITE_COLUMNS = 12;
        static constexpr int SPRITE_ROWS = 11;
        static constexpr int LAST_ROW_FRAMES = 6;
        static constexpr int FRAME_WIDTH = 696;
        static constexpr int FRAME_HEIGHT = 450;
        static constexpr float FRAME_TIME = 1.f / 24.f;

        sf::Texture m_backgroundTexture;
        sf::Sprite m_backgroundSprite;
        sf::Font m_font;
        std::vector<sf::Text> m_optionTexts;
        std::vector<Option> m_options;
        sf::VertexArray m_menuGradient;
        std::function<void(Option)> m_onOptionSelected;
        std::size_t m_selectedIndex = 0;
        std::size_t m_hoveredIndex = static_cast<std::size_t>(-1);
        float m_blinkTimer = 0.f;
        float m_frameTimer = 0.f;
        int m_frameIndex = 0;
    };
}

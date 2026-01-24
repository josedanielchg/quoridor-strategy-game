#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>
#include "app/Screen.hpp"

namespace App
{
    class HowToPlayScreen : public Screen
    {
    public:
        static constexpr const char *MUSIC_PATH = "assets/sound/main_theme.mp3";

        struct TutorialStep
        {
            enum class Speaker
            {
                Knight,
                Wizard
            };

            Speaker speaker = Speaker::Knight;
            std::string text;
            std::string boardImagePath;
        };

        HowToPlayScreen();
        bool init() override;
        void setOnBack(std::function<void()> onBack);

        void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
        void handleResize(sf::RenderWindow &window, sf::Vector2u size) override;
        void update(float dt) override;
        void render(sf::RenderWindow &window) override;
        void onEnter() override;

    private:
        const char *getMusicPath() const override { return MUSIC_PATH; }
        const char *screenName() const override { return "HowToPlayScreen"; }
        void updateLayout(const sf::View &view);
        void applyStep(std::size_t index);
        void advanceStep();
        bool loadBoardImage(const std::string &path);
        void updateDialogueText();
        sf::FloatRect computeBoardRect(const sf::View &view) const;
        sf::FloatRect computeDialogueRect(const sf::View &view) const;
        sf::FloatRect computeNameplateRect(const sf::View &view) const;
        static std::string wrapText(const std::string &input,
                                    const sf::Font &font,
                                    unsigned int characterSize,
                                    float maxWidth);
        static std::vector<TutorialStep> loadTutorialStepsFromFile(const std::string &path);

        sf::Texture m_knightBackgroundTexture;
        sf::Texture m_wizardBackgroundTexture;
        sf::Sprite m_backgroundSprite;
        sf::Texture m_boardTexture;
        sf::Sprite m_boardSprite;
        sf::Font m_font;
        sf::Text m_dialogueText;
        sf::Text m_speakerNameText;
        sf::Text m_speakerNameShadow;
        sf::ConvexShape m_continueIndicator;
        std::vector<TutorialStep> m_steps;
        std::size_t m_currentStep = 0;
        std::string m_loadedBoardPath;
        bool m_hasBoardImage = false;
        bool m_hasFont = false;
        std::function<void()> m_onBack;
        float m_blinkTimer = 0.f;
        sf::FloatRect m_boardRect;
        sf::FloatRect m_dialogueRect;
        sf::FloatRect m_nameplateRect;
    };
}

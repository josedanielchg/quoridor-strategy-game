#pragma once
#include <SFML/Graphics.hpp>

namespace App
{
    class Screen
    {
    public:
        // Base virtual destructor for safe cleanup.
        virtual ~Screen() = default;
        // Optional initialization hook; return false on failure.
        virtual bool init() { return true; }
        // Handle input events for this screen.
        virtual void handleEvent(const sf::Event &event, sf::RenderWindow &window) = 0;
        // React to window resize or view changes.
        virtual void handleResize(sf::RenderWindow &window, sf::Vector2u size) = 0;
        // Per-frame update step.
        virtual void update(float dt) = 0;
        // Render the screen content.
        virtual void render(sf::RenderWindow &window) = 0;

        // Called when the screen becomes active.
        virtual void onEnter();
        // Called when the screen is deactivated.
        virtual void onExit();

        // Default music volume for screens.
        static constexpr float DEFAULT_MUSIC_VOLUME = 40.f;

    protected:
        // Override to provide a background music path.
        virtual const char *getMusicPath() const { return nullptr; }
        // Restart the current screen music from the beginning.
        void restartMusic();
    };
}

#pragma once
#include <SFML/Audio.hpp>
#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace Audio
{
    enum class SfxId
    {
        Move,
        Hover,
        Click,
        Wall,
        Win,
        TutorialNextDialog,
        MenuOpen,
        Count
    };

    class SfxManager
    {
    public:
        
        static SfxManager &instance(); // Access the singleton instance.

        void play(SfxId id); // Play a sound effect by id.
        
        void preloadAll(); // Preload all sound buffers.
        
        void setSfxVolume(float volume); // Set the master SFX volume (0-100).
        
        void setMuted(bool muted); // Mute or unmute SFX playback.
        
        bool isMuted() const; // Check whether SFX is muted.

    private:
        
        SfxManager(); // Initialize asset paths and pool state.
        
        std::size_t indexFromId(SfxId id) const; // Map a SfxId to its array index.
        
        bool loadBuffer(SfxId id); // Load a buffer on demand.
        
        float volumeFromScale(float scale) const; // Convert a scale factor to a clamped volume.
        
        sf::Sound &acquireSound(const sf::SoundBuffer &buffer, float volumeScale); // Acquire a sound from the pool or rotate when full.
        
        void applyVolume(); // Apply current volume settings to the pool.

        static constexpr std::size_t kMaxPool = 16;
        std::array<std::string, static_cast<std::size_t>(SfxId::Count)> m_paths;
        std::array<sf::SoundBuffer, static_cast<std::size_t>(SfxId::Count)> m_buffers;
        std::array<bool, static_cast<std::size_t>(SfxId::Count)> m_loaded{};
        std::array<bool, static_cast<std::size_t>(SfxId::Count)> m_failed{};
        std::array<float, static_cast<std::size_t>(SfxId::Count)> m_volumeScales{};
        std::vector<sf::Sound> m_pool;
        std::vector<float> m_poolScales;
        std::size_t m_nextIndex = 0;
        float m_volume = 70.f;
        bool m_muted = false;
    };
}

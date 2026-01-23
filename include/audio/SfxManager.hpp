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
        static SfxManager &instance();

        void play(SfxId id);
        void preloadAll();
        void setSfxVolume(float volume);
        void setMuted(bool muted);
        bool isMuted() const;

    private:
        SfxManager();
        std::size_t indexFromId(SfxId id) const;
        bool loadBuffer(SfxId id);
        float volumeFromScale(float scale) const;
        sf::Sound &acquireSound(const sf::SoundBuffer &buffer, float volumeScale);
        void applyVolume();

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

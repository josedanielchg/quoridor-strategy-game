#include "audio/SfxManager.hpp"
#include <algorithm>
#include <iostream>

namespace Audio
{
    // Access the singleton instance for global SFX control. #
    SfxManager &SfxManager::instance()
    {
        static SfxManager manager;
        return manager;
    }

    // Initialize asset paths, pool storage, and per-sound volume scales. #
    SfxManager::SfxManager()
    {
        m_paths[indexFromId(SfxId::Move)] = "assets/sound/pawn_move.mp3";
        m_paths[indexFromId(SfxId::Hover)] = "assets/sound/hover.mp3";
        m_paths[indexFromId(SfxId::Click)] = "assets/sound/click.mp3";
        m_paths[indexFromId(SfxId::Wall)] = "assets/sound/wall_1.mp3";
        m_paths[indexFromId(SfxId::Win)] = "assets/sound/win.mp3";
        m_paths[indexFromId(SfxId::TutorialNextDialog)] = "assets/sound/next_dialog.mp3";
        m_paths[indexFromId(SfxId::MenuOpen)] = "assets/sound/click.mp3";
        m_pool.reserve(kMaxPool);
        m_poolScales.reserve(kMaxPool);
        m_volumeScales.fill(1.f);
        m_volumeScales[indexFromId(SfxId::Hover)] = 0.20f;
        m_volumeScales[indexFromId(SfxId::Wall)] = 2.f;
        m_volumeScales[indexFromId(SfxId::Move)] = 3.f;
    }

    // Play a sound effect if not muted and the buffer loads successfully. #
    void SfxManager::play(SfxId id)
    {
        if (m_muted)
            return;

        if (!loadBuffer(id))
            return;

        const float volumeScale = m_volumeScales[indexFromId(id)];
        sf::Sound &sound = acquireSound(m_buffers[indexFromId(id)], volumeScale);
        sound.setBuffer(m_buffers[indexFromId(id)]);
        sound.setVolume(volumeFromScale(volumeScale));
        sound.play();
    }

    // Load all SFX buffers upfront to catch missing assets early. #
    void SfxManager::preloadAll()
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(SfxId::Count); ++i)
        {
            loadBuffer(static_cast<SfxId>(i));
        }
    }

    // Clamp and apply the master SFX volume across active sounds. #
    void SfxManager::setSfxVolume(float volume)
    {
        m_volume = std::max(0.f, std::min(volume, 100.f));
        applyVolume();
    }

    // Toggle the mute flag and refresh the pool volume state. #
    void SfxManager::setMuted(bool muted)
    {
        m_muted = muted;
        applyVolume();
    }

    // Report whether SFX playback is currently muted. #
    bool SfxManager::isMuted() const
    {
        return m_muted;
    }

    // Convert a SfxId enum to its backing array index. #
    std::size_t SfxManager::indexFromId(SfxId id) const
    {
        return static_cast<std::size_t>(id);
    }

    // Lazily load a sound buffer and cache failures to avoid retries. #
    bool SfxManager::loadBuffer(SfxId id)
    {
        const std::size_t index = indexFromId(id);
        if (m_loaded[index])
            return true;
        if (m_failed[index])
            return false;

        const std::string &path = m_paths[index];
        if (path.empty())
        {
            std::cerr << "SFX path missing for id " << index << "\n";
            m_failed[index] = true;
            return false;
        }

        if (!m_buffers[index].loadFromFile(path))
        {
            std::cerr << "Failed to load SFX: " << path << "\n";
            m_failed[index] = true;
            return false;
        }

        m_loaded[index] = true;
        return true;
    }

    // Compute scaled volume with mute and clamp logic applied. #
    float SfxManager::volumeFromScale(float scale) const
    {
        if (m_muted)
            return 0.f;
        return std::max(0.f, std::min(m_volume * scale, 100.f));
    }

    // Reuse a stopped sound or rotate through the pool when full. #
    sf::Sound &SfxManager::acquireSound(const sf::SoundBuffer &buffer, float volumeScale)
    {
        for (std::size_t i = 0; i < m_pool.size(); ++i)
        {
            if (m_pool[i].getStatus() == sf::SoundSource::Status::Stopped)
            {
                m_poolScales[i] = volumeScale;
                return m_pool[i];
            }
        }

        if (m_pool.size() < kMaxPool)
        {
            m_pool.emplace_back(buffer);
            m_poolScales.push_back(volumeScale);
            return m_pool.back();
        }

        sf::Sound &sound = m_pool[m_nextIndex];
        m_poolScales[m_nextIndex] = volumeScale;
        m_nextIndex = (m_nextIndex + 1) % m_pool.size();
        sound.stop();
        return sound;
    }

    // Apply current volume settings to every pooled sound. #
    void SfxManager::applyVolume()
    {
        for (std::size_t i = 0; i < m_pool.size(); ++i)
            m_pool[i].setVolume(volumeFromScale(m_poolScales[i]));
    }
}

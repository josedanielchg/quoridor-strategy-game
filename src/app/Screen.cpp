#include "app/Screen.hpp"
#include "resources/ResourceLoader.hpp"
#include <SFML/Audio.hpp>
#include <string>

namespace App
{
    namespace
    {
        struct SharedMusicState
        {
            sf::Music music;
            std::string currentPath;
        };

        SharedMusicState &sharedMusic()
        {
            static SharedMusicState state;
            return state;
        }
    }

    void Screen::onEnter()
    {
        const char *path = getMusicPath();
        if (!path || path[0] == '\0')
            return;

        SharedMusicState &state = sharedMusic();
        const std::string pathStr(path);

        if (state.currentPath == pathStr)
        {
            if (state.music.getStatus() != sf::SoundSource::Status::Playing)
                state.music.play();
            return;
        }

        state.music.stop();
        Resources::openMusicInto(state.music,
                                 pathStr,
                                 screenName(),
                                 "Screen music");

        state.currentPath = pathStr;
        state.music.setLooping(true);
        state.music.setVolume(DEFAULT_MUSIC_VOLUME);
        state.music.play();
    }

    void Screen::onExit()
    {
        const char *path = getMusicPath();
        if (!path || path[0] == '\0')
            return;

        SharedMusicState &state = sharedMusic();
        if (state.currentPath != path)
            return;

        if (state.music.getStatus() == sf::SoundSource::Status::Playing)
            state.music.pause();
    }

    void Screen::restartMusic()
    {
        const char *path = getMusicPath();
        if (!path || path[0] == '\0')
            return;

        SharedMusicState &state = sharedMusic();
        if (state.currentPath != path)
            return;

        state.music.stop();
        state.music.setLooping(true);
        state.music.setVolume(DEFAULT_MUSIC_VOLUME);
        state.music.play();
    }
}

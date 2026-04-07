#pragma once

#include <string>
#include <memory>

struct ma_engine;
struct ma_sound;

namespace PulseStudio {

    enum class Musics
    {
        None,
        Minecraft,
        The_Garden,
        Kora_Lullaby,
        At_Journey_End,
        Amateur_Cartography,
        Inverted_Island,
        The_Chasm,
        The_Spire,
        Water_Palace
    };

    class MusicPlayer
    {
    public:
        MusicPlayer();
        ~MusicPlayer();

        void SetMusic(Musics music);
        void SetVolume(unsigned short volume);
        void OnUpdate(float deltaTime);

        MusicPlayer(const MusicPlayer&) = delete;
        MusicPlayer(const MusicPlayer&&) = delete;
        MusicPlayer& operator=(const MusicPlayer&) = delete;

    private:
        bool LoadMusic(Musics music);

        std::string GetMusicPath(Musics music) const;

        Musics m_currentMusic = Musics::Kora_Lullaby;
        float m_targetVolume = 0.7f;

        std::unique_ptr<ma_engine> m_engine;
        std::unique_ptr<ma_sound>  m_sound;
    };

}
#include "pspch.h"
#include "MusicPlayer.h"
#include "Log.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace PulseStudio {

	MusicPlayer::MusicPlayer()
	{
		m_engine = std::make_unique<ma_engine>();
		ma_result result = ma_engine_init(nullptr, m_engine.get());
		if (result != MA_SUCCESS)
		{
			PS_CORE_ERROR(std::format("Failed to initialize audio engine: {}", (int)result));
			if (result == -10) // MA_INVALID_FILE
			{
				PS_CORE_ERROR("This might indicate a problem with the audio file path or format.");
			}
			m_engine.reset();
		}
		else
		{
			PS_CORE_INFO("Audio engine initialized.");
		}
	}

	MusicPlayer::~MusicPlayer()
	{
		if (m_sound)
		{
			ma_sound_stop(m_sound.get());
			m_sound.reset();
		}

		if (m_engine)
		{
			ma_engine_uninit(m_engine.get());
		}
	}

	void MusicPlayer::SetMusic(Musics music)
	{
		PS_CORE_INFO(std::format("SetMusic called with enum {}", (int)music));
		if (music == m_currentMusic)
		{
			return;
		}

		m_currentMusic = music;

		if (music == Musics::None)
		{
			PS_INFO("Turn off the background music.");
			return;
		}

		if (!LoadMusic(music))
		{
			PS_ERROR(std::format("Failed to load music for enum {}", (int)music));
			m_currentMusic = Musics::None;
			return;
		}
		else
		{
			PS_INFO(std::format("Loaded music for enum {}.", (int)music));
		}

		SetVolume(m_targetVolume * 1000);

		ma_sound_start(m_sound.get());
		PS_INFO(std::format("Started playing music: {}", GetMusicPath(music)));
	}
	void MusicPlayer::SetVolume(unsigned short volume)
	{
		if (volume > 1000) volume = 1000;
		m_targetVolume = volume / 1000.0f;

		if (m_sound)
		{
			ma_sound_set_volume(m_sound.get(), m_targetVolume);
		}
		PS_INFO(std::format("Change the volume of the background music to {}.", m_targetVolume * 100));
	}

	void MusicPlayer::OnUpdate(float deltaTime)
	{
		(void)deltaTime;
	}


	bool MusicPlayer::LoadMusic(Musics music)
	{
		std::string path = GetMusicPath(music);
		if (path.empty())
		{
			PS_CORE_ERROR(std::format("Music path is empty for enum {}", (int)music));
			return false;
		}
		PS_CORE_INFO("Attempting to load music from: {}", path);

		std::ifstream file(path, std::ios::binary);
		if (!file.is_open())
		{
			PS_CORE_ERROR("File not found or cannot be opened: {}", path);
			return false;
		}
		PS_CORE_INFO("File exists and is accessible.");

		auto newSound = std::make_unique<ma_sound>();
		ma_result result = ma_sound_init_from_file(m_engine.get(), path.c_str(), 0, nullptr, nullptr, newSound.get());
		if (result != MA_SUCCESS)
		{
			PS_CORE_ERROR("Failed to load sound file '{}', error code: {}", path, (int)result);
			return false;
		}
		else
		{
			PS_INFO(std::format("Loaded sound file '{}'.", path));
		}

		ma_sound_set_volume(newSound.get(), m_targetVolume);

		ma_sound_set_looping(newSound.get(), MA_TRUE);

		m_sound = std::move(newSound);
		return true;
	}

	std::string MusicPlayer::GetMusicPath(Musics music) const
	{
		const std::string basePath = "H:/Projects/CppProject/Pulse-Studio/Resources/Musics";

		switch (music)
		{
		case Musics::Minecraft:
			return basePath + "C418 - Minecraft.mp3";
			break;
		case Musics::The_Garden:
			return basePath + "Stafford Bawler - The Garden.mp3";
			break;
		case Musics::Kora_Lullaby:
			return basePath + "Todd Baker - Kora Lullaby.mp3";
			break;
		case Musics::At_Journey_End:
			return basePath + "Grigori - At Journey's End.mp3";
			break;
		case Musics::Amateur_Cartography:
			return basePath + "Obfusc - Amateur Cartography.mp3";
			break;
		case Musics::Inverted_Island:
			return basePath + "Obfusc - Inverted Island.mp3";
			break;
		case Musics::The_Chasm:
			return basePath + "Stafford Bawler - The Chasm.mp3";
			break;
		case Musics::The_Spire:
			return basePath + "Stafford Bawler - The Spire.mp3";
			break;
		case Musics::Water_Palace :
			return basePath + "Stafford Bawler - Water Palace.mp3";
			break;
		default:
			return "";
		}
	}

}
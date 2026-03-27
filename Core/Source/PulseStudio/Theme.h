#pragma once

namespace PulseStudio
{

	enum class Theme // 10 items
	{
		// Light
		Light = 0,
		Cool_Breeze,
		Icy_Mint,
		Sand,
		Ice,
		// Dark
		Dark,
		Cool_Slate,
		Moonlight,
		Forest,
		Grape
	};

	class ThemeManager
	{
	public:
		static Theme GetCurrentTheme() { return s_CurrentTheme; }
		static void SetTheme(Theme theme) { s_CurrentTheme = theme; }
	private:
		ThemeManager() = default;

		ThemeManager(const ThemeManager&) = delete;
		ThemeManager& operator=(const ThemeManager&) = delete;
		ThemeManager(ThemeManager&&) = delete;
		ThemeManager& operator=(ThemeManager&&) = delete;
		ThemeManager& operator=(const ThemeManager&&) = delete;

		static Theme s_CurrentTheme;
	};

}

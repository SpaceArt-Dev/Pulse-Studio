#pragma once
#include "PulseStudio/Window.h"

struct GLFWwindow;

namespace PulseStudio {

	class WindowsWindow : public Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void Shutdown();
		void Init(const WindowProps& props);

		void OnUpdate() override;

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		static Window* Create(const WindowProps& props = WindowProps());

		inline virtual void* GetNativeWindow() const override;

		void SetUnsemi_transparency(double value) override;
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			double unsemi_transparency;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

		unsigned int m_Width, m_Height;
	};

}

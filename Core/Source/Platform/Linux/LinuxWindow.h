#pragma once
#include "PulseStudio/Window.h"

struct GLFWwindow;

namespace PulseStudio {

    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();

        void OnUpdate() override;

        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) override;
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const override { return m_Window; }

        void SetUnsemi_transparency(unsigned int value);
    private:
        void Init(const WindowProps& props);
        void Shutdown();

        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}
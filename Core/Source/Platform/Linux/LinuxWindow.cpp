#include "pspch.h"
#include "LinuxWindow.h"

#ifdef PS_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glad/glad.h>

#ifdef PS_PLATFORM_LINUX
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
#endif

#include "PulseStudio/Log.h"
#include "PulseStudio/Events/ApplicationEvent.h"
#include "PulseStudio/Events/KeyEvent.h"
#include "PulseStudio/Events/MouseEvent.h"

namespace PulseStudio {

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        PS_CORE_ERROR(std::format("GLFW Error ({0}): {1}", error, description));
    }

    LinuxWindow::LinuxWindow(const WindowProps& props)
    {
        Init(props);
    }

    LinuxWindow::~LinuxWindow()
    {
        Shutdown();
    }

    void LinuxWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        PS_CORE_INFO(std::format("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height));

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            PS_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PS_CORE_ASSERT(status, "Could not initialize Glad!");
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;
                WindowResizeEvent event(width, height);
                if (data.EventCallback)
                    data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                if (data.EventCallback)
                    data.EventCallback(event);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    if (data.EventCallback) data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    if (data.EventCallback) data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    if (data.EventCallback) data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button, xpos, ypos);
                    if (data.EventCallback)
                        data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button, xpos, ypos);
                    if (data.EventCallback)
                        data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                if (data.EventCallback) data.EventCallback(event);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseMovedEvent event((float)xPos, (float)yPos);
                if (data.EventCallback) data.EventCallback(event);
            });
    }

    void LinuxWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void LinuxWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    unsigned int LinuxWindow::GetWidth() const { return m_Data.Width; }
    unsigned int LinuxWindow::GetHeight() const { return m_Data.Height; }

    void LinuxWindow::SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

    void LinuxWindow::SetVSync(bool enabled)
    {
        m_Data.VSync = enabled;
        glfwSwapInterval(enabled ? 1 : 0);
    }

    bool LinuxWindow::IsVSync() const { return m_Data.VSync; }

    void LinuxWindow::SetUnsemi_transparency(unsigned int value)
    {
#ifdef PS_PLATFORM_LINUX
        m_Data.unsemi_transparency = value;

        Display* display = glfwGetX11Display();
        Window x11Window = glfwGetX11Window(m_Window);
        if (!display || !x11Window) return;

        unsigned long Value = (unsigned long)(value * 0xFFFFFFFF);

        Atom atom = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);
        if (atom != None)
        {
            XChangeProperty(display, x11Window, atom, XA_CARDINAL, 32,
                PropModeReplace, (unsigned char*)&Value, 1);
            XFlush(display);
        }
#endif
    }

}
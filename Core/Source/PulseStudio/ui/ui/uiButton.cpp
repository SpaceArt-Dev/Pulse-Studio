#include "pspch.h"
#include "uiButton.h"
#include "TextRenderer.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace PulseStudio {

    uiButton::uiButton(const std::string& text, float x, float y, float width, float height, ButtonStyles style)
        : m_Text(text), m_X(x), m_Y(y), m_Width(width), m_Height(height), m_ButtonStyle(style)
    {
    }

    uiButton::~uiButton() {}

    void uiButton::Draw(float parentX, float parentY) const
    {
        float absX = parentX + m_X;
        float absY = parentY + m_Y;

        float r = m_Color[0], g = m_Color[1], b = m_Color[2];

        if (m_Hovered)
        {
            r = std::min(r + 0.1f, 1.0f);
            g = std::min(g + 0.1f, 1.0f);
            b = std::min(b + 0.1f, 1.0f);
        }

        if (m_ButtonStyle != ButtonStyles::NoBackgroundOrLine)
        {
            if (m_ButtonStyle != ButtonStyles::NoBackground)
            {
                glColor4f(r, g, b, m_Color[3]);
                glBegin(GL_QUADS);
                glVertex2f(absX, absY);
                glVertex2f(absX + m_Width, absY);
                glVertex2f(absX + m_Width, absY + m_Height);
                glVertex2f(absX, absY + m_Height);
                glEnd();
            }
            else
            {
                if (m_Hovered)
                {
                    glColor4f(r, g, b, m_Color[3] - 0.1f);
                    glBegin(GL_QUADS);
                    glVertex2f(absX, absY);
                    glVertex2f(absX + m_Width, absY);
                    glVertex2f(absX + m_Width, absY + m_Height);
                    glVertex2f(absX, absY + m_Height);
                    glEnd();
                }
            }

            if (m_ButtonStyle != ButtonStyles::NoLine)
            {
                glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
                glLineWidth(1.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(absX, absY);
                glVertex2f(absX + m_Width, absY);
                glVertex2f(absX + m_Width, absY + m_Height);
                glVertex2f(absX, absY + m_Height);
                glEnd();
            }
            else
            {
                glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
                glLineWidth(1.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(absX, absY);
                glVertex2f(absX + m_Width, absY);
                glVertex2f(absX + m_Width, absY + m_Height);
                glVertex2f(absX, absY + m_Height);
                glEnd();
            }
        }
        else
        {
            if (m_Hovered)
            {
                glColor4f(r, g, b, m_Color[3] - 0.1f);
                glBegin(GL_QUADS);
                glVertex2f(absX, absY);
                glVertex2f(absX + m_Width, absY);
                glVertex2f(absX + m_Width, absY + m_Height);
                glVertex2f(absX, absY + m_Height);
                glEnd();
            }
        }

        float textWidth = TextRenderer::Get().GetTextWidth(m_Text);
        float textHeight = TextRenderer::Get().GetTextHeight();
        float textX = absX + (m_Width - textWidth) / 2;
        float textY = absY + (m_Height - textHeight) / 2;

        float tr, tg, tb;
        if (ThemeManager::IsDarkTheme)
        {
            tr = tg = tb = 0.95f;
        }
        else 
        {
            tr = tg = tb = 0.05f;
        }
        TextRenderer::Get().DrawText(m_Text, textX, textY, tr, tr, tr, 1.0f);
    }

    bool uiButton::IsPointInside(float px, float py, float parentX, float parentY) const
    {
        float absX = parentX + m_X;
        float absY = parentY + m_Y;
        return (px >= absX && px <= absX + m_Width && py >= absY && py <= absY + m_Height);
    }

    void uiButton::OnUpdate(float parentX, float parentY, bool parentVisible)
    {
        if (!parentVisible) return;
        Draw(parentX, parentY);
    }

    bool uiButton::OnEvent(Event& event, float parentX, float parentY, bool parentVisible)
    {
        if (!parentVisible) return false;

        if (event.GetEventType() == EventType::MouseMoved)
        {
            MouseMovedEvent& e = (MouseMovedEvent&)event;
            bool inside = IsPointInside(e.GetX(), e.GetY(), parentX, parentY);
            if (inside != m_Hovered)
                m_Hovered = inside;
            return false;
        }
        else if (event.GetEventType() == EventType::MouseButtonPressed)
        {
            MouseButtonPressedEvent& e = (MouseButtonPressedEvent&)event;
            if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT && IsPointInside(e.GetMouseX(), e.GetMouseY(), parentX, parentY))
            {
                if (m_Callback) m_Callback();
                return true;
            }
        }
        return false;
    }

}
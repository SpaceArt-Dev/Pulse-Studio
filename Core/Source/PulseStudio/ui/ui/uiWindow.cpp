#include "pspch.h"
#include "uiWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "PulseStudio/Application.h"
#include "PulseStudio/Log.h"

#include "PulseStudio/Events/Event.h"
#include "PulseStudio/Events/ApplicationEvent.h"
#include "PulseStudio/Events/KeyEvent.h"
#include "PulseStudio/Events/MouseEvent.h"

namespace PulseStudio
{

	uiWindow::uiWindow(std::string name)
		: m_name(name)
	{
		m_name = name;
		if (ThemeManager::GetCurrentTheme() == Theme::Dark || ThemeManager::GetCurrentTheme() == Theme::Cool_Slate || ThemeManager::GetCurrentTheme() == Theme::Moonlight || ThemeManager::GetCurrentTheme() == Theme::Forest || ThemeManager::GetCurrentTheme() == Theme::Grape)
		{
			PS_CORE_TRACE("Use dark ui theme.");
			SetStyle(true); // Dark
		}
		else if (ThemeManager::GetCurrentTheme() == Theme::Light || ThemeManager::GetCurrentTheme() == Theme::Cool_Breeze || ThemeManager::GetCurrentTheme() == Theme::Icy_Mint || ThemeManager::GetCurrentTheme() == Theme::Sand || ThemeManager::GetCurrentTheme() == Theme::Ice)
		{
			PS_CORE_TRACE("Use light ui theme.");
			SetStyle(false); // Light
		}
		else
		{
			PS_CORE_ERROR("Unknow this theme!");
			PS_CORE_TRACE("Use dark ui theme!");
			SetStyle(true); // Dark
		}

		static bool fontLoaded = false;
		if (!fontLoaded)
		{
			TextRenderer::Get().LoadFont("H:/Projects/CppProject/Pulse-Studio/Resources/Fonts/Ubuntu-L.ttf", 24.0f);
			fontLoaded = true;
		}
	}

	uiWindow::~uiWindow()
	{
		for (auto *btn : m_Buttons)
			delete btn;
		m_Buttons.clear();
	}

	void uiWindow::OnAttach()
	{
		PS_CORE_TRACE("uiWindow attached.");
	}

	void uiWindow::OnDetach()
	{
		PS_CORE_TRACE("uiWindow detached.");
	}

	void uiWindow::OnUpdate(float deltaTime)
	{
		if (!m_IsVisible)
			return;
		DrawContent();
	}

	void uiWindow::DrawContent()
	{
		Application &app = Application::Get();
		int width = (int)app.GetWindow().GetWidth();
		int height = (int)app.GetWindow().GetHeight();

		if (width == 0 || height == 0)
			return;

		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		glViewport(0, 0, width, height);
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		glPushMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		const int shadowOffset = 5;
		const int shadowSteps = 7;
		const float startAlpha = 0.1f;
		const float endAlpha = 0.0f;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (int i = 0; i < shadowSteps; ++i)
		{
			float t = (float)i / (shadowSteps - 1);
			float alpha = startAlpha * (1.0f - t) + endAlpha * t;
			float offset = shadowOffset * (1.0f - t);

			glColor4f(0.0f, 0.0f, 0.0f, alpha);
			glBegin(GL_POLYGON);

			glVertex2f(m_RectX, m_RectY);
			glVertex2f(m_RectX + m_RectWidth + offset, m_RectY);
			glVertex2f(m_RectX + m_RectWidth + offset, m_RectY + m_RectHeight + offset);
			glVertex2f(m_RectX, m_RectY + m_RectHeight + offset);
			glEnd();
		}

		int scissorX = (int)m_RectX;
		int scissorY = (int)(height - (m_RectY + m_RectHeight));
		int scissorW = (int)m_RectWidth;
		int scissorH = (int)m_RectHeight;
		glEnable(GL_SCISSOR_TEST);
		glScissor(scissorX, scissorY, scissorW, scissorH);

		glColor4f(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
		glBegin(GL_POLYGON);
		glVertex2f(m_RectX, m_RectY);
		glVertex2f(m_RectX + m_RectWidth, m_RectY);
		glVertex2f(m_RectX + m_RectWidth, m_RectY + m_RectHeight);
		glVertex2f(m_RectX, m_RectY + m_RectHeight);
		glEnd();

		glColor4f(((m_Color[0] + 0.001f) > 1.0f ? 1.0f : (m_Color[0] + 0.001f)), ((m_Color[1] + 0.001f) > 1.0f ? 1.0f : (m_Color[1] + 0.001f)), ((m_Color[2] + 0.001f) > 1.0f ? 1.0f : (m_Color[2] + 0.001f)), 1.0f);
		glBegin(GL_QUADS);
		glVertex2f(m_RectX, m_RectY);
		glVertex2f(m_RectX + m_RectWidth, m_RectY);
		glVertex2f(m_RectX + m_RectWidth, m_RectY + 35.0f);
		glVertex2f(m_RectX, m_RectY + 35.0f);
		glEnd();

		DrawTitle();
		DrawResizeGrip();

		float closeX = m_RectX + m_RectWidth - m_CloseButtonSize - 5;
		float closeY = m_RectY + (30.0f - m_CloseButtonSize) / 2;

		if (m_CloseButtonHovered)
		{
			glColor4f(1.0f, 0.2f, 0.2f, 0.9f);
			glBegin(GL_QUADS);
			glVertex2f(closeX, closeY);
			glVertex2f(closeX + m_CloseButtonSize, closeY);
			glVertex2f(closeX + m_CloseButtonSize, closeY + m_CloseButtonSize);
			glVertex2f(closeX, closeY + m_CloseButtonSize);
			glEnd();
		}

		float crossR, crossG, crossB;
		if (m_IsDarkTheme)
		{
			crossR = 1.0f;
			crossG = 1.0f;
			crossB = 1.0f;
		}
		else
		{
			crossR = 0.0f;
			crossG = 0.0f;
			crossB = 0.0f;
		}
		glColor4f(crossR, crossG, crossB, 1.0f);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glVertex2f(closeX + 5, closeY + 5);
		glVertex2f(closeX + m_CloseButtonSize - 5, closeY + m_CloseButtonSize - 5);
		glVertex2f(closeX + m_CloseButtonSize - 5, closeY + 5);
		glVertex2f(closeX + 5, closeY + m_CloseButtonSize - 5);
		glEnd();

		for (auto *btn : m_Buttons)
		{
			btn->OnUpdate(m_RectX, m_RectY, m_IsVisible);
		}

		glDisable(GL_SCISSOR_TEST);

		glPopMatrix();
		glPopAttrib();
	}

	bool uiWindow::OnEvent(Event &event)
	{
		if (!m_IsVisible)
			return false;

		for (auto it = m_Buttons.rbegin(); it != m_Buttons.rend(); ++it)
		{
			if ((*it)->OnEvent(event, m_RectX, m_RectY, m_IsVisible))
			{
				event.m_Handled = true;
				return false;
			}
		}

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent &e)
			{
			float mx = e.GetMouseX();
			float my = e.GetMouseY();

			bool inTitleBar = (mx >= m_RectX && mx <= m_RectX + m_RectWidth && my >= m_RectY && my <= m_RectY + 30.0f);

			float closeX = m_RectX + m_RectWidth - m_CloseButtonSize - 10;
			float closeY = m_RectY + (30.0f - m_CloseButtonSize) / 2;
			
			bool inCloseButton = (mx >= closeX && mx <= closeX + m_CloseButtonSize && my >= closeY && my <= closeY + m_CloseButtonSize);
			
			if (inCloseButton)
			{
				PS_CORE_WARN("Close \"{}\" ui window!", m_name);
				m_IsVisible = false;
				return true;
			}

			if (inTitleBar && e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				m_IsDragging = true;
				m_DragStartX = mx;
				m_DragStartY = my;
				m_WindowStartX = m_RectX;
				m_WindowStartY = m_RectY;
				return true;
			}

			ResizeEdge edge = GetResizeEdge(mx, my);
			if (edge != ResizeEdge::None)
			{
				m_IsResizing = true;
				m_ResizeEdge = edge;
				m_ResizeStartX = mx;
				m_ResizeStartY = my;
				m_ResizeStartRectX = m_RectX;
				m_ResizeStartRectY = m_RectY;
				m_ResizeStartWidth = m_RectWidth;
				m_ResizeStartHeight = m_RectHeight;
				return true;
			}

			if (IsInResizeZone(mx, my) && e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				m_IsResizing = true;
				m_ResizeStartX = mx;
				m_ResizeStartY = my;
				m_ResizeStartWidth = m_RectWidth;
				m_ResizeStartHeight = m_RectHeight;
				return true;
			}

			return false; });

		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent &e)
			{
				float mx = e.GetX();
				float my = e.GetY();

				float closeX = m_RectX + m_RectWidth - m_CloseButtonSize - 5;
				float closeY = m_RectY + (30.0f - m_CloseButtonSize) / 2;
				bool inClose = (e.GetX() >= closeX && e.GetX() <= closeX + m_CloseButtonSize &&
					e.GetY() >= closeY && e.GetY() <= closeY + m_CloseButtonSize);
				if (inClose != m_CloseButtonHovered)
				{
					m_CloseButtonHovered = inClose;
				}

				if (m_IsDragging)
				{
					float dx = e.GetX() - m_DragStartX;
					float dy = e.GetY() - m_DragStartY;
					m_RectX = m_WindowStartX + dx;
					m_RectY = m_WindowStartY + dy;
					PS_CORE_TRACE(std::format("Dragging: dx = {}, dy = {}", e.GetX() - m_DragStartX, e.GetY() - m_DragStartY));
					return true;
				}

				bool inResizeZone = IsInResizeZone(mx, my);
				SetResizeCursor(inResizeZone);

				if (m_IsResizing)
				{
					float dx = mx - m_ResizeStartX;
					float dy = my - m_ResizeStartY;
					float newX = m_ResizeStartRectX;
					float newY = m_ResizeStartRectY;
					float newW = m_ResizeStartWidth;
					float newH = m_ResizeStartHeight;

					switch (m_ResizeEdge)
					{
					case ResizeEdge::Left:
						newW = m_ResizeStartWidth - dx;
						if (newW >= m_MinWidth)
						{
							newX = m_ResizeStartRectX + dx;
							m_RectWidth = newW;
							m_RectX = newX;
						}
						break;
					case ResizeEdge::Right:
						newW = m_ResizeStartWidth + dx;
						if (newW >= m_MinWidth)
							m_RectWidth = newW;
						break;
					case ResizeEdge::Top:
						newH = m_ResizeStartHeight - dy;
						if (newH >= m_MinHeight)
						{
							newY = m_ResizeStartRectY + dy;
							m_RectHeight = newH;
							m_RectY = newY;
						}
						break;
					case ResizeEdge::Bottom:
						newH = m_ResizeStartHeight + dy;
						if (newH >= m_MinHeight)
							m_RectHeight = newH;
						break;
					case ResizeEdge::TopLeft:
						newW = m_ResizeStartWidth - dx;
						newH = m_ResizeStartHeight - dy;
						if (newW >= m_MinWidth && newH >= m_MinHeight)
						{
							m_RectX = m_ResizeStartRectX + dx;
							m_RectY = m_ResizeStartRectY + dy;
							m_RectWidth = newW;
							m_RectHeight = newH;
						}
						break;
					case ResizeEdge::TopRight:
						newW = m_ResizeStartWidth + dx;
						newH = m_ResizeStartHeight - dy;
						if (newW >= m_MinWidth && newH >= m_MinHeight)
						{
							m_RectY = m_ResizeStartRectY + dy;
							m_RectWidth = newW;
							m_RectHeight = newH;
						}
						break;
					case ResizeEdge::BottomLeft:
						newW = m_ResizeStartWidth - dx;
						newH = m_ResizeStartHeight + dy;
						if (newW >= m_MinWidth && newH >= m_MinHeight)
						{
							m_RectX = m_ResizeStartRectX + dx;
							m_RectWidth = newW;
							m_RectHeight = newH;
						}
						break;
					case ResizeEdge::BottomRight:
						newW = m_ResizeStartWidth + dx;
						newH = m_ResizeStartHeight + dy;
						if (newW >= m_MinWidth && newH >= m_MinHeight)
						{
							m_RectWidth = newW;
							m_RectHeight = newH;
						}
						break;
					default:
						break;
					}
					return true;
				}

				ResizeEdge edge = GetResizeEdge(mx, my);
				UpdateResizeCursor(edge);

				return false; });

		dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent &e)
		{
			if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				m_IsDragging = false;
				m_IsResizing = false;
				UpdateResizeCursor(ResizeEdge::None);
				return true;
			}
			return false;
		});

		return false;
	}

	void uiWindow::SetSize(float x, float y, float width, float height)
	{
		if (this == nullptr)
		{
			PS_CORE_ERROR("SetSize called on null uiWindow!");
			return;
		}
		m_RectX = x;
		m_RectY = y;
		m_RectWidth = width;
		m_RectHeight = height;
	}

	void uiWindow::SetStyle(bool isDark)
	{
		if (isDark)
		{
			m_Color[0] = 0.07f;
			m_Color[1] = 0.07f;
			m_Color[2] = 0.15f;
			m_Color[3] = 1.0f;

			m_IsDarkTheme = true;
		}
		else
		{
			m_Color[0] = 0.83f;
			m_Color[1] = 0.83f;
			m_Color[2] = 0.9f;
			m_Color[3] = 1.0f;

			m_IsDarkTheme = false;
		}
	}

	void uiWindow::DrawTitle() const
	{
		float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
		if (!m_IsDarkTheme)
		{
			r = 0.1f;
			g = 0.1f;
			b = 0.1f;
		}
		TextRenderer::Get().DrawText(m_name, m_RectX + 12, m_RectY + 8, r, g, b, a);
	}

	bool uiWindow::IsInResizeZone(float mx, float my) const
	{
		float zoneX = m_RectX + m_RectWidth - 15.0f;
		float zoneY = m_RectY + m_RectHeight - 15.0f;

		return (mx >= zoneX && mx <= m_RectX + m_RectWidth &&
				my >= zoneY && my <= m_RectY + m_RectHeight);
	}

	void uiWindow::SetResizeCursor(bool isResizeZone)
	{
		GLFWwindow *glfwWindow = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
		if (isResizeZone)
			glfwSetCursor(glfwWindow, glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR));
		else
			glfwSetCursor(glfwWindow, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
	}

	ResizeEdge uiWindow::GetResizeEdge(float mx, float my) const
	{
		const int edge = m_EdgeSize;
		const float left = m_RectX;
		const float right = m_RectX + m_RectWidth;
		const float top = m_RectY;
		const float bottom = m_RectY + m_RectHeight;

		bool onLeft = (mx >= left && mx <= left + edge);
		bool onRight = (mx >= right - edge && mx <= right);
		bool onTop = (my >= top && my <= top + edge);
		bool onBottom = (my >= bottom - edge && my <= bottom);

		if (onLeft && onTop)
			return ResizeEdge::TopLeft;
		if (onLeft && onBottom)
			return ResizeEdge::BottomLeft;
		if (onRight && onTop)
			return ResizeEdge::TopRight;
		if (onRight && onBottom)
			return ResizeEdge::BottomRight;
		if (onLeft)
			return ResizeEdge::Left;
		if (onRight)
			return ResizeEdge::Right;
		if (onTop)
			return ResizeEdge::Top;
		if (onBottom)
			return ResizeEdge::Bottom;

		return ResizeEdge::None;
	}

	void uiWindow::UpdateResizeCursor(ResizeEdge edge)
	{
		GLFWwindow *glfwWin = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
		GLFWcursor *cursor = nullptr;

		switch (edge)
		{
		case ResizeEdge::Left:
		case ResizeEdge::Right:
			cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
			break;
		case ResizeEdge::Top:
		case ResizeEdge::Bottom:
			cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
			break;
		case ResizeEdge::TopLeft:
		case ResizeEdge::BottomRight:
			cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
			break;
		case ResizeEdge::TopRight:
		case ResizeEdge::BottomLeft:
			cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
			break;
		default:
			cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
			break;
		}
		glfwSetCursor(glfwWin, cursor);
	}

	void uiWindow::DrawResizeGrip() const
	{
		if (!m_IsVisible)
			return;

		float gripSize = 12.0f;
		float startX = m_RectX + m_RectWidth - gripSize;
		float startY = m_RectY + m_RectHeight - gripSize;

		if (ThemeManager::IsDarkTheme())
		{
			glColor4f(0.7f, 0.7f, 0.7f, 0.5f);
		}
		else
		{
			glColor4f(0.3f, 0.3f, 0.3f, 0.5f);
		}
		glBegin(GL_TRIANGLES);
		glVertex2f(startX, m_RectY + m_RectHeight);
		glVertex2f(m_RectX + m_RectWidth, startY);
		glVertex2f(m_RectX + m_RectWidth, m_RectY + m_RectHeight);
		glEnd();
	}

	void uiWindow::AddButton(uiButton *button)
	{
		if (button)
		{
			m_Buttons.push_back(button);
		}
	}

}

#include "pspch.h"
#include "uiLayer.h"
#include "uiWindow.h"
#include "PulseStudio/Log.h"

namespace PulseStudio {

	uiLayer::uiLayer()
		: Layer("UILayer")
	{
	}

	uiLayer::~uiLayer()
	{
		for (auto* win : m_Windows)
		{
			delete win;
		}
		m_Windows.clear();
	}

	void uiLayer::OnAttach()
	{
		PS_CORE_INFO("uiLayer attached.");

		uiWindow* fileExplorer = new uiWindow("FileExplorer");
		fileExplorer->SetSize(100, 150, 500, 700);
		m_Windows.push_back(fileExplorer);

		for (auto* win : m_Windows)
		{
			win->OnAttach();
		}
	}

	void uiLayer::OnDetach()
	{
		for (auto* win : m_Windows)
		{
			win->OnDetach();
		}
	}

	void uiLayer::OnUpdate(float deltaTime)
	{
		for (auto* win : m_Windows)
		{
			win->OnUpdate(deltaTime);
		}
	}

	void uiLayer::OnEvent(Event& event)
	{
		for (auto it = m_Windows.rbegin(); it != m_Windows.rend(); ++it)
		{
			(*it)->OnEvent(event);

			if (event.m_Handled)
			{
				return;
			}
		}
	}

	void uiLayer::AddWindow(uiWindow* window)
	{
		if (window)
		{
			m_Windows.push_back(window);
			window->OnAttach();
		}
	}

}
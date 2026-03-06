#include <Pulse.h>
#include <iostream>

#include "PulseStudio/Events/KeyEvent.h"

class ExampleLayer : public PulseStudio::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate(float deltaTime) override
	{
	}

	void OnEvent(PulseStudio::Event& event) override
	{
	}
};

class SandboxApp : public PulseStudio::Application
{
public:
	SandboxApp()
	{
	}
	~SandboxApp()
	{
	}
};

PulseStudio::Application* PulseStudio::CreateApplication()
{
	return new SandboxApp();
}

enum ConsoleStatus
{
	Show,
	Hide
};

static void ChangeConsoleStatus(ConsoleStatus status)
{
	HWND hwnd = GetConsoleWindow();
	if (status == ConsoleStatus::Show)
	{
		ShowWindow(hwnd, SW_SHOW); // Show Console
		PS_INFO("Console is Showed.");
	}
	else
	{
		ShowWindow(hwnd, SW_HIDE); // Hide Console
		PS_INFO("Console is Hided.");
	}
}

int main()
{
	ChangeConsoleStatus(ConsoleStatus::Show);

	PulseStudio::Application* app = PulseStudio::CreateApplication();

	app->Run();

	delete app;
	return 0;
}

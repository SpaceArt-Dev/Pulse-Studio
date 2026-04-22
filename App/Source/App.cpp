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

	bool OnEvent(PulseStudio::Event& event) override
	{
		return false;
	}
};

class SandboxApp : public PulseStudio::Application
{
public:
	SandboxApp()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new PulseStudio::uiLayer());
	}
	~SandboxApp()
	{
	}
};

PulseStudio::Application* PulseStudio::CreateApplication()
{
	return new SandboxApp();
}

int main()
{
	PulseStudio::Application* app = PulseStudio::CreateApplication();

	app->Run();

	delete app;
	return 0;
}

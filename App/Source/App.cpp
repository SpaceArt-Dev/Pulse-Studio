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

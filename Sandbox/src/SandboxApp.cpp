#include <Fire.h>
#include <Fire/Core/EntryPoint.h>

//#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Fire::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{}
};

Fire::Application* Fire::CreateApplication()
{
	return new Sandbox();
}

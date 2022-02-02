#include <Fire.h>
#include <Fire/Core/EntryPoint.h>

#include "GameLayer.h"

class Sandbox : public Fire::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{
	}
};

Fire::Application* Fire::CreateApplication()
{
	return new Sandbox();
}
#pragma once
#include "Fire/Core/Base.h"

// 'extern' is implicit in function declarations anyway,
// but indicates to us that the declaration occurs outside of Fire
extern Fire::Application* Fire::CreateApplication();

int main(int argc, char** argv)
{
	Fire::Log::Init();
	
	FR_PROFILE_BEGIN_SESSION("Startup", "FireProfile-Startup.json");
	auto app = Fire::CreateApplication();
	FR_PROFILE_END_SESSION();
	
	FR_PROFILE_BEGIN_SESSION("Runtime", "FireProfile-Runtime.json");
	app->Run();
	FR_PROFILE_END_SESSION();

	FR_PROFILE_BEGIN_SESSION("Shutdown", "FireProfile-Shutdown.json");
	delete app;
	FR_PROFILE_END_SESSION();

	return 0;
}

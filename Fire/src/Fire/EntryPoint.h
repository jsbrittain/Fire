#pragma once

// 'extern' is implicit in function declarations anyway,
// but indicates to us that the declaration occurs outside of Fire
extern Fire::Application* Fire::CreateApplication();

int main(int argc, char** argv)
{
	Fire::Log::Init();
	FR_CORE_WARN("Initialised log.");
	int a = 5;
	FR_INFO("hello var={0}",a);
	auto app = Fire::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

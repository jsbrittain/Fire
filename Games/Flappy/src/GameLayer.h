#pragma once

#include "Fire.h"

#include "Level.h"
#include <../vendor/imgui-docking/imgui.h>
//#include <imgui/imgui.h>

class GameLayer : public Fire::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Fire::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Fire::Event& e) override;
	bool OnMouseButtonPressed(Fire::MouseButtonPressedEvent& e);
	bool OnWindowResize(Fire::WindowResizeEvent& e);
private:
	void CreateCamera(uint32_t width, uint32_t height);
private:
	Fire::Scope<Fire::OrthographicCamera> m_Camera;
	Level m_Level;
	ImFont* m_Font;
	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play = 0, MainMenu = 1, GameOver = 2
	};

	GameState m_State = GameState::MainMenu;
};
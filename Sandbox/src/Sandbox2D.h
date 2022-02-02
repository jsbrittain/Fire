#pragma once

#include "Fire.h"

class Sandbox2D : public Fire::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Fire::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Fire::Event& e) override;

	float GetFPS();

private:
	float m_fps = 0.0f;
	bool m_VsyncState = true, m_VsyncGuiState = true;	// Current state and ImGui state, so only switch when changes
	
private:
	Fire::OrthographicCameraController m_CameraController;

	Fire::Ref<Fire::VertexArray> m_SquareVA;
	Fire::Ref<Fire::Shader> m_FlatColorShader;
	Fire::Ref<Fire::Framebuffer> m_Framebuffer;

	Fire::Ref<Fire::Texture2D> m_CheckerboardTexture;
	Fire::Ref<Fire::Texture2D> m_SpriteSheet;
	Fire::Ref<Fire::SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Fire::Ref<Fire::SubTexture2D>> s_TextureMap;
	std::unordered_map<char, glm::vec2> s_TextureMapSize;
};

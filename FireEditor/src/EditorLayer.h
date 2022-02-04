#pragma once

#include "Fire.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Fire {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		float GetFPS();

	private:
		float m_fps = 0.0f;
		bool m_VsyncState = true, m_VsyncGuiState = true;	// Current state and ImGui state, so only switch when changes
		
	private:
		OrthographicCameraController m_CameraController;

		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;

		Entity m_CameraEntity;
 		Entity m_SecondCamera;

 		bool m_PrimaryCamera = true;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
		std::unordered_map<char, glm::vec2> s_TextureMapSize;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0,0};

		// Panels
 		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}

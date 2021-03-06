#include "EditorLayer.h"
#include <../vendor/imgui-docking/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "Hazel/Scene/SceneSerializer.h"

namespace Fire {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f/720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		FR_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		
		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		// Construct Scene
		m_ActiveScene = CreateRef<Scene>();


		// Entity
		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f,1.0f,0.5f,1.0f});

		/*std::cout << trans[0][0] << ", " << trans[0][1] << ", " << trans[0][2] << ", " << trans[0][3] << std::endl;
		std::cout << trans[1][0] << ", " << trans[1][1] << ", " << trans[1][2] << ", " << trans[1][3] << std::endl;
		std::cout << trans[2][0] << ", " << trans[2][1] << ", " << trans[2][2] << ", " << trans[2][3] << std::endl;
		std::cout << trans[3][0] << ", " << trans[3][1] << ", " << trans[3][2] << ", " << trans[3][3] << std::endl;*/

		m_SquareEntity = square;		// Keep track of square

		auto square2 = m_ActiveScene->CreateEntity("Red Square");
		square2.AddComponent<SpriteRendererComponent>(glm::vec4{1.0f,0.0f,0.0f,1.0f});
		auto& trans2 = square2.GetComponent<TransformComponent>().Translation;
		trans2.x = -1.0f; trans2.z = -0.5f;

		auto square3 = m_ActiveScene->CreateEntity("Blue Square");
		square3.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f,0.0f,1.0f,1.0f});
		auto& trans3 = square3.GetComponent<TransformComponent>().Translation;
		trans3.x = 1.0f; trans3.z = 0.5f;



		// Camera
		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

 		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
 		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
 		cc.Primary = false;

 		class CameraController : public ScriptableEntity
 		{
 		public:
 			virtual void OnCreate() override
 			{
 				auto& translation = GetComponent<TransformComponent>().Translation;
 				translation.x = rand() % 10 - 5.0f;
 			}

 			virtual void OnDestroy() override
 			{
 			}

 			virtual void OnUpdate(Timestep ts) override
 			{
 				auto& translation = GetComponent<TransformComponent>().Translation;
 				float speed = 5.0f;

 				if (Input::IsKeyPressed(Key::A))
 					translation.x -= speed * ts;
 				if (Input::IsKeyPressed(Key::D))
 					translation.x += speed * ts;
 				if (Input::IsKeyPressed(Key::W))
 					translation.y += speed * ts;
 				if (Input::IsKeyPressed(Key::S))
 					translation.y -= speed * ts;
 			}
 		};

 		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
 		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

 		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		FR_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		FR_PROFILE_FUNCTION();

		// Check state changes due to ImGui
		if ( m_VsyncState != m_VsyncGuiState )
		{
			m_VsyncState = m_VsyncGuiState;
			Application::Get().GetWindow().SetVSync( m_VsyncState );
		}

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		if ( m_ViewportFocused) 
			m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Update scene
		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();

		// FPS
		m_fps = 0.9f*m_fps + 0.1f/ts;		// Running average
	}

	void EditorLayer::OnImGuiRender()
	{
		FR_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
 		float minWinSizeX = style.WindowMinSize.x;
 		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				/*if (ImGui::MenuItem("Serialize"))
 				{
 					SceneSerializer serializer(m_ActiveScene);
 					serializer.Serialize("assets/scenes/Example.hazel");
 				}

 				if (ImGui::MenuItem("Deserialize"))
 				{
 					SceneSerializer serializer(m_ActiveScene);
 					serializer.Deserialize("assets/scenes/Example.hazel");
 				}*/
 				
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Statistics");

		auto stats = Renderer2D::GetStats();
	 	ImGui::Text("Renderer2D Stats:");
	 	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	 	ImGui::Text("Quads: %d", stats.QuadCount);
	 	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	 	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	 	ImGui::Checkbox("VSync: ", &m_VsyncGuiState);
	 	ImGui::Text("Frame timing: %.1f msecs (%.1f fps)", 1000/m_fps, m_fps);

	 	if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
 		{
 			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
 			m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
 		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();		// Get viewport size
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{0,1}, ImVec2{1,0});
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();		// Dockspace
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

}

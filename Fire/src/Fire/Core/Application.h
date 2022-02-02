#pragma once

#include "Fire/Core/Base.h"
#include "Fire/Core/Window.h"
#include "Fire/Core/LayerStack.h"
#include "Fire/Events/Event.h"
#include "Fire/Events/ApplicationEvent.h"

#include "Fire/ImGui/ImGuiLayer.h"

#include "Fire/Core/Timestep.h"

namespace Fire {

	class FIRE_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		void Close();
		
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}

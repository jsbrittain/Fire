#include "frpch.h"
#include "Application.h"

#include "Fire/Log.h"

#include "Fire/Renderer/Renderer.h"
#include "Input.h"

#include <GLFW/glfw3.h>

namespace Fire {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		FR_CORE_ASSERT(!s_Instance,"Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		//m_Window->SetVSync(false);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	// If event is of type WindowCloseEvent then call OnWindowClose
		
		// Log ALL events received
		//FR_CORE_TRACE("{0}",e);

		// Move through layer stack backwards for events
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while ( m_Running )
		{
			// Calculate 'dt' (last frame time)
			float time = (float)glfwGetTime();								////// Should be a platform independent call
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

        	// Move through layer stack forwards for rendering
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			// Render queue
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}

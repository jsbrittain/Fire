#include "frpch.h"
#include "Fire/Core/Application.h"

#include "Fire/Core/Log.h"

#include "Fire/Renderer/Renderer.h"
#include "Fire/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Fire {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		FR_PROFILE_FUNCTION();

		FR_CORE_ASSERT(!s_Instance,"Application already exists!");
		s_Instance = this;
		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(FR_BIND_EVENT_FN(Application::OnEvent));
		//m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		FR_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		FR_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		FR_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		FR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(FR_BIND_EVENT_FN(Application::OnWindowClose));	// If event is of type WindowCloseEvent then call OnWindowClose
		dispatcher.Dispatch<WindowResizeEvent>(FR_BIND_EVENT_FN(Application::OnWindowResize));

		// Move through layer stack backwards for events
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		FR_PROFILE_FUNCTION();

		while ( m_Running )
		{
			FR_PROFILE_SCOPE("RunLoop");

			// Calculate 'dt' (last frame time)
			float time = (float)glfwGetTime();								////// Should be a platform independent call
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

        	// Move through layer stack forwards for rendering
        	if ( !m_Minimized )
        	{
        		{
    				FR_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
        		}

				// Render queue
				m_ImGuiLayer->Begin();
				{
					FR_PROFILE_SCOPE("LayerStack OnGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		FR_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return true;
		}
		m_Minimized = false;
#ifndef __APPLE__			////// Resizing on Mac seems to work perfectly fine without this, but this lines breaks that. NEEDED ON WINDOWS? ///
		Renderer::OnWindowResize(e.GetWidth(),e.GetHeight());
#endif
		return false;
	}

}

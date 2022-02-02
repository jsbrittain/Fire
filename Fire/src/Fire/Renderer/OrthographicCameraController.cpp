#include "frpch.h"
#include "Fire/Renderer/OrthographicCameraController.h"

#include "Fire/Core/Input.h"
#include "Fire/Core/KeyCodes.h"

namespace Fire {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		  m_Bounds({-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel}),
		  m_Camera( m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top ),
		  m_Rotation(rotation)
	{
	}
	
	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		FR_PROFILE_FUNCTION();

		// Move camera in response to AD-keys
		if (Input::IsKeyPressed(FR_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(FR_KEY_D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
 		}
		// Move camera in response to WS-keys
		if (Input::IsKeyPressed(FR_KEY_W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
 		}
		else if (Input::IsKeyPressed(FR_KEY_S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
 		}
		// Camera rotation in response to QE-keys
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(FR_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(FR_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		FR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(FR_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		FR_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		FR_PROFILE_FUNCTION();
		
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		CalculateView();
		return false;
	}

}

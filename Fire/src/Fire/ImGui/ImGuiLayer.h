#pragma once

#include "Fire/Core/Layer.h"

#include "Fire/Events/ApplicationEvent.h"
#include "Fire/Events/KeyEvent.h"
#include "Fire/Events/MouseEvent.h"

namespace Fire {

	class FIRE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}
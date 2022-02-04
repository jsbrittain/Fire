#pragma once

#include <glm/glm.hpp>
#include "Fire/Core/KeyCodes.h"
#include "Fire/Core/MouseCodes.h"

namespace Fire {

	class FIRE_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}

#pragma once

#include "Fire/Core/Base.h"
#include "Fire/Core/Log.h"
#include "Fire/Scene/Scene.h"
#include "Fire/Scene/Entity.h"

namespace Fire {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}
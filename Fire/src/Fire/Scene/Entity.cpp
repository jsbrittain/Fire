#include "frpch.h"
#include "Entity.h"

namespace Fire {
	
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}
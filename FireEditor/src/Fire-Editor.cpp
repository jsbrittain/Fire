#include <Fire.h>
#include <Fire/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Fire {

	class FireEditor : public Application
	{
	public:
		FireEditor()
			: Application("Fire Editor")
		{
			PushLayer(new EditorLayer());
		}

		~FireEditor()
		{}
	};

	Application* CreateApplication()
	{
		return new FireEditor();
	}

}

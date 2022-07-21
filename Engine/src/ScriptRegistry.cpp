#include "ScriptRegistry.h"

namespace Engine {
	ScriptRegistry* Engine::ScriptRegistry::Get()
	{
		static ScriptRegistry* instance = nullptr;
		if (instance == nullptr)
			instance = new ScriptRegistry;

		return instance;
	}
	ScriptRegistry::ScriptRegistry()
	{
	}
}

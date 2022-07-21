#pragma once

#include <string>
#include <map>
#include <vector>

#include "Common.h"
#include "Components.h"

namespace Engine
{
	class ScriptRegistry
	{
	public:
		PUBLIC static ScriptRegistry* Get();
		std::map<std::string, ScriptComponent> m_ScriptsComponents;
		std::vector<std::string> m_ScriptNames;
	private:
		PUBLIC ScriptRegistry();
	};
}
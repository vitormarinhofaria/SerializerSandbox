#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <map>

#include <Components.h>
#include "GameDll.h"
namespace Editor {
	class GameProject
	{
	public:
		static GameProject* New(std::string_view projectName, std::string_view projectDir, std::string_view templateDir);
		static GameProject* Open(std::string_view projectDir);

		void ConfigureCode();
		void CompileCode();
		void ReloadCode();
		
		std::map<std::string, ScriptComponent> m_RegisteredScripts;
		std::vector<const char*> m_RegisteredScriptNames;
		std::string m_ProjectDir;
		std::string m_ProjectName;
	private:
		GameProject();
		GameDll m_projectDll;
		std::string m_binOutputDir;
	};
}
#pragma once
#include <Windows.h>
#include <string_view>
#include <Components.h>

namespace Editor
{
	typedef void (*InitFunc)(std::map<std::string, ScriptComponent>*);

	class GameDll
	{
	public:
		explicit GameDll(std::string_view path);
		GameDll();
		~GameDll();
		void Reload(std::map<std::string, ScriptComponent>*);
		std::string m_dllPath;
		void FreeUntilError();
	private:
		HMODULE m_instance;
		InitFunc m_initFunc;
	};
}


#include <Windows.h>
#include <string_view>
#include <string>

#include "GameDll.h"

namespace Editor
{
	GameDll::GameDll(std::string_view path) : m_instance(nullptr), m_initFunc(nullptr), m_dllPath(path)
	{
	}
	GameDll::GameDll() : m_instance(nullptr), m_initFunc(nullptr) {}

	GameDll::~GameDll()
	{
		FreeUntilError();
	}
	void GameDll::Reload(std::map<std::string, ScriptComponent>* map)
	{
		FreeUntilError();
		HMODULE libModule = LoadLibraryA(m_dllPath.c_str());
		if (libModule != NULL)
		{
			m_instance = libModule;

			InitFunc func = reinterpret_cast<InitFunc>(GetProcAddress(m_instance, "GameInit"));
			if (func != nullptr)
			{
				m_initFunc = func;
				m_initFunc(map);
			}
		}
	}

	void GameDll::FreeUntilError()
	{
		if (m_instance != NULL) {
			BOOL freeResult = 0;
			do {
				freeResult = FreeLibrary(m_instance);
			} while (freeResult != 0);
			m_instance = NULL;
		}
	}
}
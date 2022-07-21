#include <string_view>
#include <filesystem>
#include <format>

#include "ScriptRegistry.h"

#include "GameProject.h"
namespace Editor {
	GameProject* GameProject::New(std::string_view projectName, std::string_view projectDir, std::string_view templateName)
	{

		bool createDirResult = std::filesystem::create_directories(projectDir);
		if (!createDirResult)
			return nullptr;

		std::filesystem::copy(templateName, projectDir, std::filesystem::copy_options::recursive);
		std::filesystem::copy(std::format("{}/Engine", SOURCE), std::format("{}/Engine", projectDir), std::filesystem::copy_options::recursive);

		GameProject* project = new GameProject;
		project->m_ProjectDir = projectDir;
		project->m_ProjectName = projectName;

		std::filesystem::current_path(projectDir);
		project->ConfigureCode();
		project->CompileCode();
		project->ReloadCode();

		return project;
	}
	GameProject* GameProject::Open(std::string_view projectDir)
	{
		GameProject* project = new GameProject;
		project->m_ProjectDir = projectDir;

		auto lastSlash = projectDir.find_last_of("/");
		if (lastSlash != std::string::npos)
		{
			project->m_ProjectName = projectDir.substr(lastSlash);
		}
		std::filesystem::current_path(projectDir);
		project->m_binOutputDir = "./out";
		//project->ConfigureCode();
		project->CompileCode();
		project->ReloadCode();

		return project;
	}

	void GameProject::ConfigureCode()
	{
		m_binOutputDir = "./out";
		auto* vcpkg_root = std::getenv("VCPKG_ROOT");
		std::string cmakeCommand = std::format("cmake -S . -B {} -DCMAKE_TOOLCHAIN_FILE={}/scripts/buildsystems/vcpkg.cmake -G Ninja", m_binOutputDir, vcpkg_root);
		std::cout << "Cmake configure command: " << cmakeCommand << '\n';
		system(cmakeCommand.c_str());
	}
	void GameProject::CompileCode()
	{
		m_projectDll.FreeUntilError();
		std::string compileCommand = std::format("cmake --build {} -t GameModule -j8", m_binOutputDir);
		system(compileCommand.c_str());
	}
	void GameProject::ReloadCode()
	{
		m_RegisteredScripts.clear();
		m_RegisteredScriptNames.clear();

		m_projectDll.m_dllPath = std::format("{}/bin/GameModule.dll", m_binOutputDir);
		m_projectDll.Reload(&m_RegisteredScripts);

		Engine::ScriptRegistry::Get()->m_ScriptsComponents = m_RegisteredScripts;
		for (auto& kv : m_RegisteredScripts) {
			m_RegisteredScriptNames.push_back(kv.first.c_str());
			Engine::ScriptRegistry::Get()->m_ScriptNames.push_back(kv.first);
		}
	}

	GameProject::GameProject()
	{

	}
}
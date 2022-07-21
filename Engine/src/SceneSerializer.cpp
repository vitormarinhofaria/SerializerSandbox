#include "SceneSerializer.h"
#include "Components.h"
#include "Entity.h"
#include "ScriptRegistry.h"

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>
#include <fstream>

namespace YAML
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& vec)
		{
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& vec)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			return true;
		}
	};
}

void SerializeEntity(YAML::Emitter& yaml, Entity& entity)
{
	yaml << YAML::BeginMap;
	yaml << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<UUIDComponent>()->beg;

	TagComponent* tag = entity.GetComponent<TagComponent>();
	if (tag != nullptr)
	{
		yaml << YAML::Key << "TagComponent" << YAML::BeginMap;
		yaml << YAML::Key << "Tag" << YAML::Value << std::string(tag->Tag());
		yaml << YAML::EndMap;
	}

	TransformComponent* transform = entity.GetComponent<TransformComponent>();
	if (transform != nullptr)
	{
		yaml << YAML::Key << "TransformComponent" << YAML::BeginMap;
		yaml << YAML::Key << "Position" << YAML::Value << transform->Position;
		yaml << YAML::Key << "Rotation" << YAML::Value << transform->Rotation;
		yaml << YAML::Key << "Scale" << YAML::Value << transform->Scale;
		yaml << YAML::EndMap;
	}

	ScriptComponent* script = entity.GetComponent<ScriptComponent>();
	if (script != nullptr)
	{
		yaml << YAML::Key << "ScriptComponent" << YAML::BeginMap;
		yaml << YAML::Key << "Behavior" << YAML::Value << script->m_BehaviorName.c_str();
		yaml << YAML::EndMap;
	}
	yaml << YAML::EndMap;
}

PUBLIC std::string SceneToString(Scene& scene)
{
	YAML::Emitter yaml;

	yaml << YAML::BeginMap;
	yaml << YAML::Key << "Scene" << YAML::Value << scene.m_Name;
	yaml << YAML::Key << "Entities";
	yaml << YAML::Value << YAML::BeginSeq;

	scene.m_World.each([&](entt::entity entity) {
		Entity ent = { scene.m_World,  entity };
		SerializeEntity(yaml, ent);
		});

	yaml << YAML::EndSeq;
	yaml << YAML::EndMap;
	return yaml.c_str();
}

PUBLIC Scene SceneFromString(std::string_view sceneString)
{
	auto input = YAML::Load(sceneString.data());
	Scene scene = { input["Scene"].as<std::string>() };

	auto entities = input["Entities"];
	for (auto entity : entities)
	{
		Entity ent = scene.CreateEntity();
		auto uuid = entity["Entity"].as<uint64_t>();
		ent.GetComponent<UUIDComponent>()->beg = uuid;

		auto tag = entity["TagComponent"]["Tag"].as<std::string>();
		ent.GetComponent<TagComponent>()->Tag(tag.c_str());

		auto transformNode = entity["TransformComponent"];
		TransformComponent* transform = ent.GetComponent<TransformComponent>();
		transform->Position = transformNode["Position"].as<glm::vec3>();
		transform->Rotation = transformNode["Rotation"].as<glm::vec3>();
		transform->Scale = transformNode["Scale"].as<glm::vec3>();

		if (entity["ScriptComponent"])
		{
			auto scriptName = entity["ScriptComponent"]["Behavior"].as<std::string>();

			auto& scriptComponent = Engine::ScriptRegistry::Get()->m_ScriptsComponents[scriptName];
			ent.AddComponent(scriptComponent);
		}
	}
	return scene;
}

PUBLIC void SceneToTextFile(Scene& scene, std::string_view filePath)
{
	std::ofstream ofs(filePath.data());

	ofs << SceneToString(scene);
	ofs.flush();
	ofs.close();
}

PUBLIC Scene SceneFromTextFile(std::string_view filePath)
{
	std::ifstream ifs(filePath.data());
	std::stringstream ss;
	std::string line;

	while (std::getline(ifs, line)) {
		ss << line << '\n';
	}
	std::string sceneString = ss.str();
	return SceneFromString(sceneString);
}

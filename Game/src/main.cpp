#include <Scripts.h>
#include <Components.h>
#include <Common.h>

extern "C" __declspec(dllexport) void GameInit(std::map<std::string, ScriptComponent>*map)
{
    register_class<Camera>(map);
    register_class<Player>(map);
    register_class<Din>(map);
    register_class<Vida>(map);
}

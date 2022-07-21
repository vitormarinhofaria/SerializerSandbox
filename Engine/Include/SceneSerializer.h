#pragma once

#include "Common.h"
#include "Scene.h"

#include <string>
#include <string_view>

PUBLIC std::string SceneToString(Scene& scene);
PUBLIC Scene SceneFromString(std::string_view sceneString);

PUBLIC void SceneToTextFile(Scene& scene, std::string_view filePath);
PUBLIC Scene SceneFromTextFile(std::string_view filePaht);
#pragma once

#include "Common.h"

namespace Engine
{
	class Shader
	{
	public:
		PUBLIC static Shader* Create(const char* vsShaderCode, const char* psShaderCode);
		PUBLIC virtual void Bind() = 0;
		PUBLIC virtual void* GetId() = 0;
	};
}
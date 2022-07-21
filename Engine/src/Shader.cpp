#include "Shader.h"
#include "Renderer.h"

#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/ShaderOpenGL.h"
#endif

namespace Engine
{
	Shader* Shader::Create(const char* vsShaderCode, const char* psShaderCode)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::OpenGL:
		{
			return new ShaderOpenGL(vsShaderCode, psShaderCode);
		}
		}
		return nullptr;
	}
}

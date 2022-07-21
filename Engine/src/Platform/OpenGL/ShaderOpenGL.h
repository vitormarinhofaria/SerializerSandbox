#pragma once

#include "Shader.h"
#include "glad/gl.h"

namespace Engine
{
	class ShaderOpenGL : public Shader
	{
	public:
		PUBLIC ShaderOpenGL(const char* vsShaderSource, const char* psSahderSource);
		PUBLIC virtual void Bind() override;
		PUBLIC virtual void* GetId() override;
	private:
		GLuint m_Vs;
		GLuint m_Ps;
		GLuint m_Program;
	};
}
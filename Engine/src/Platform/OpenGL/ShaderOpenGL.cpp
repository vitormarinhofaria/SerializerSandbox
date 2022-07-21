#include "ShaderOpenGL.h"
#include <string>
#include <iostream>

namespace Engine
{
    void checkCompileErrors(unsigned int shader, const std::string& type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
	ShaderOpenGL::ShaderOpenGL(const char* vsShaderSource, const char* psShaderSource) : m_Vs(glCreateShader(GL_VERTEX_SHADER)), m_Ps(glCreateShader(GL_VERTEX_SHADER))
	{

		//m_Vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_Vs, 1, &vsShaderSource, nullptr);
		glCompileShader(m_Vs);
        checkCompileErrors(m_Vs, "VERTEX");

		//m_Ps = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_Ps, 1, &psShaderSource, nullptr);
		glCompileShader(m_Ps);
        checkCompileErrors(m_Vs, "FRAGMENT");

		m_Program = glCreateProgram();
		glAttachShader(m_Program, m_Vs);
		glAttachShader(m_Program, m_Ps);
		glLinkProgram(m_Program);

		glDeleteShader(m_Vs);
		glDeleteShader(m_Ps);
	}
	void ShaderOpenGL::Bind()
	{
		glUseProgram(m_Program);
	}
    PUBLIC void* ShaderOpenGL::GetId()
    {
        return (void*)m_Program;
    }
}
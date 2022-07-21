#pragma once

#include "glad/gl.h"
#include "Buffer.h"

namespace Engine
{
	class VertexBufferOpenGL : public VertexBuffer 
	{
	public:
		PUBLIC VertexBufferOpenGL(float* vertices, size_t stride, size_t vertsSize, uint32_t* indices, size_t indicesSize);
		PUBLIC void Bind() override;
		virtual PUBLIC size_t IndicesCount() override;
		virtual PUBLIC size_t VerticesCount() override;
	private:
		GLuint m_BufferObject;
		GLuint m_IndexObject;
		GLuint m_AttribObject;

		size_t m_IndicesCount = 0;
		size_t m_VerticesCount = 0;
	};

	class IndexBufferOpenGL : public IndexBuffer
	{
	public:
		PUBLIC IndexBufferOpenGL(uint32_t* data, size_t count);
		PUBLIC void Bind() override;
		PUBLIC size_t Count() override;
	private:
		GLuint m_Buffer;
		size_t m_Count;
	};
}
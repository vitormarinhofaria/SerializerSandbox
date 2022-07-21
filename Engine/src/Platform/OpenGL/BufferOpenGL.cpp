#include "BufferOpenGL.h"
#include <Windows.h>

namespace Engine
{
	VertexBufferOpenGL::VertexBufferOpenGL(float* vertices, size_t stride, size_t vertsSize, uint32_t* indices, size_t indicesSize) 
	{
		m_VerticesCount = (vertsSize / sizeof(float)) / stride;
		m_IndicesCount = indicesSize / sizeof(uint32_t);

		glGenVertexArrays(1, &m_AttribObject);
		glGenBuffers(1, &m_BufferObject);
		glGenBuffers(1, &m_IndexObject);

		glBindVertexArray(m_AttribObject);

		glBindBuffer(GL_ARRAY_BUFFER, m_BufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertsSize, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_BufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBufferOpenGL::Bind()
	{
		glBindVertexArray(m_AttribObject);
	}
	size_t VertexBufferOpenGL::IndicesCount()
	{
		return m_IndicesCount;
	}
	size_t VertexBufferOpenGL::VerticesCount()
	{
		return m_VerticesCount;
	}
	IndexBufferOpenGL::IndexBufferOpenGL(uint32_t* data, size_t count) : m_Count(count)
	{
		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
	}
	void IndexBufferOpenGL::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
	}
	size_t IndexBufferOpenGL::Count()
	{
		return m_Count;
	}
}

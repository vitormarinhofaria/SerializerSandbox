#include "Buffer.h"

#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/BufferOpenGL.h"
#endif

#include "Renderer.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::Create(float* vertices, size_t stride, size_t vertsSize, uint32_t* indices, size_t indicesSize)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::OpenGL:
		{
			return new VertexBufferOpenGL(vertices, stride, vertsSize, indices, indicesSize);
		}
		}
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* data, size_t count)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::OpenGL:
		{
			return new IndexBufferOpenGL(data, count);
		}
		}
		return nullptr;
	}
}

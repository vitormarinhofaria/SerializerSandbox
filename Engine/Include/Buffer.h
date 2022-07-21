#pragma once
#include "Common.h"
#include <cstdint>
namespace Engine
{
	class VertexBuffer
	{
	public:
		/// <summary>
		/// Creates a Vertex Buffer for the current Renderer api.
		/// </summary>
		/// <param name="vertices">pointer to vertex data</param>
		/// <param name="stride">vertex stride</param>
		/// <param name="vertsSize">size (in bytes) of vertex data</param>
		/// <param name="indices">pointer to index data</param>
		/// <param name="indicesSize">size (in bytes) of index data</param>
		/// <returns></returns>
		PUBLIC static VertexBuffer* Create(float* vertices, size_t stride, size_t vertsSize, uint32_t* indices, size_t indicesSize);
		PUBLIC virtual void Bind() = 0;
		PUBLIC virtual size_t IndicesCount() = 0;
		PUBLIC virtual size_t VerticesCount() = 0;
	};

	class IndexBuffer
	{
	public:
		PUBLIC static IndexBuffer* Create(uint32_t* data, size_t cound);
		PUBLIC virtual void Bind() = 0;
		PUBLIC virtual size_t Count() = 0;
	};
}
#pragma once

#include "Common.h"
#include "Scene.h"

namespace Engine 
{
	enum RendererApi {
		None, OpenGL, Vulkan, D3D11, D3D12
	};

	class Renderer
	{
	public:
		PUBLIC static Renderer* Get();
		PUBLIC static void Init(RendererApi api, void* data);
		PUBLIC static RendererApi GetApi();
		
		PUBLIC virtual void Resolution(Vec2<int> resolution) = 0;
		PUBLIC virtual Vec2<int> Resolution() = 0;

		PUBLIC virtual void* Framebuffer() = 0;
		PUBLIC virtual void* Depthbuffer() = 0;
		PUBLIC virtual void* Stencilbuffer() = 0;
		/// <summary>
		/// Render to it's framebuffer
		/// </summary>
		PUBLIC virtual void Render(Scene& scene) = 0;
		/// <summary>
		/// Get the texture for presenting to screen
		/// </summary>
		PUBLIC virtual void* Present() = 0;
	private:
		PUBLIC Renderer();
		PUBLIC ~Renderer();

		friend class RendererOpenGL;
	};
}
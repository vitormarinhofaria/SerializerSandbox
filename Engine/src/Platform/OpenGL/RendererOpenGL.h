#pragma once
#include "Renderer.h"
#include "glad/gl.h"

namespace Engine
{
	class RendererOpenGL : public Renderer
	{
		PUBLIC void Resolution(Vec2<int> resolution) override;
		PUBLIC Vec2<int> Resolution() override;

		PUBLIC void* Framebuffer() override;
		PUBLIC void* Depthbuffer() override;
		PUBLIC void* Stencilbuffer() override;
		PUBLIC void Render(Scene& scene) override;
		PUBLIC void* Present() override;

	private:
		friend class Renderer;
		explicit PUBLIC RendererOpenGL(void* window);
		PUBLIC ~RendererOpenGL();

		PUBLIC void CreateFramebuffer(Vec2<int> size);

		GLuint m_Framebuffer = 0;
		GLuint m_Depthbuffer = 0;
		GLuint m_Stencilbuffer = 0;

		GLuint m_FramebufferTexture = 0;
		GLuint m_DepthStencilTexture = 0;
		Vec2<int> m_FramebufferSize;

		void* m_Window = nullptr;
	};
}
#include "Renderer.h"
#include "Components.h"
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/RendererOpenGL.h"
#endif
	
namespace Engine
{
	static Renderer* instance = nullptr;
	static RendererApi Api = None;

	PUBLIC RendererApi Renderer::GetApi()
	{
		return Api;
	}

	void Renderer::Init(RendererApi api, void* data)
	{
		switch (api)
		{
		case RendererApi::OpenGL: {
#ifdef PLATFORM_OPENGL
			instance = new RendererOpenGL(data);
			Api = RendererApi::OpenGL;
#endif		break;
		}
		}
	}

	Renderer* Renderer::Get()
	{
		return instance;
	}
	Renderer::Renderer() {}
	Renderer::~Renderer() {}
}

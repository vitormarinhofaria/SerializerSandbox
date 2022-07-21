#include "RendererOpenGL.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Engine
{
	void RendererOpenGL::CreateFramebuffer(Vec2<int> size)
	{
		m_FramebufferSize = size;

		glCreateFramebuffers(1, &m_Framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_FramebufferTexture);
		glBindTexture(GL_TEXTURE_2D, m_FramebufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FramebufferTexture, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthStencilTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthStencilTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); //(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, size.x, size.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilTexture, 0);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		//glGenRenderbuffers(1, &m_Depthbuffer);
		//glBindRenderbuffer(GL_RENDERBUFFER, m_Depthbuffer);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Depthbuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RendererOpenGL::Render(Scene& scene)
	{
		glViewport(0, 0, m_FramebufferSize.x, m_FramebufferSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_STENCIL_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClearColor(0.5, 0.4, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glm::mat4 camViewProj = glm::mat4(1.0);

		auto cameraEntity = scene.GetMainCamera();
		if (scene.ValidEntity(cameraEntity)) {
			auto* camera = cameraEntity.GetComponent<CameraComponent>();
			if (camera != nullptr)
			{
				TransformComponent* cameraTransform = cameraEntity.GetComponent<TransformComponent>();
				if (cameraTransform != nullptr)
				{
					auto camView = camera->GetView(cameraTransform->Position);
					camViewProj = camera->m_Projection * camView;
				}
			}
		}

		auto view = scene.m_World.view<MeshComponent, TransformComponent>();
		for (auto [entity, mesh, transform] : view.each())
		{
			mesh.m_Shader->Bind();
			mesh.m_VertexBuffer->Bind();

			glm::mat4 modelMat = glm::mat4(1.0);
			
			modelMat = glm::translate(modelMat, transform.Position);
			modelMat = glm::scale(glm::mat4(1.0), transform.Scale) * modelMat;
			modelMat = glm::rotate(glm::mat4(1.0), glm::radians(transform.Rotation.z), { 0,0,1 }) * modelMat;
			modelMat = glm::rotate(glm::mat4(1.0), glm::radians(transform.Rotation.y), { 0,1,0 }) * modelMat;
			modelMat = glm::rotate(glm::mat4(1.0), glm::radians(transform.Rotation.x), { 1,0,0 }) * modelMat;
			
			glm::mat4 mvp = camViewProj * modelMat;

			GLuint mvpLoc = glGetUniformLocation((GLuint)mesh.m_Shader->GetId(), "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
			glDrawElements(GL_TRIANGLES, mesh.m_VertexBuffer->IndicesCount(), GL_UNSIGNED_INT, 0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void* RendererOpenGL::Present()
	{
		return (void*)m_FramebufferTexture;
	}

	void RendererOpenGL::Resolution(Vec2<int> resolution)
	{
		m_FramebufferSize = resolution;
		glDeleteFramebuffers(1, &m_Framebuffer);
		glDeleteTextures(1, &m_FramebufferTexture);
		glDeleteTextures(1, &m_DepthStencilTexture);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		CreateFramebuffer(resolution);
		glViewport(0, 0, resolution.x, resolution.y);
	}
	Vec2<int> RendererOpenGL::Resolution()
	{
		return m_FramebufferSize;
	}
	void* RendererOpenGL::Framebuffer()
	{
		return (void*)m_Framebuffer;
	}
	void* RendererOpenGL::Depthbuffer()
	{
		return nullptr;
	}
	void* RendererOpenGL::Stencilbuffer()
	{
		return nullptr;
	}
	RendererOpenGL::RendererOpenGL(void* window) : m_Framebuffer(0), m_Depthbuffer(0), m_FramebufferTexture(0), m_Stencilbuffer(0)
	{
		m_Window = window;
		gladLoadGL(glfwGetProcAddress);
		glViewport(0, 0, 800, 600);
		CreateFramebuffer({ 800, 600 });
	}
	RendererOpenGL::~RendererOpenGL()
	{
	}
}

#include "ez/Core/Base.h"
#include "ez/Graphics/API/OpenGL/GL_API.h"
#include "ez/Graphics/API/OpenGL/GL_Shader.h"
#include "ez/Graphics/API/OpenGL/GL_MappedStreamBuffer.h"
#include "ez/Graphics/API/OpenGL/GL_TextureArray.h"
#include "ez/Graphics/API/OpenGL/GL_DebugOutput.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.inl>
#include <GLFW/glfw3.h>

namespace ez::gfx {
	GL_API::GL_API() {
		gladLoadGL(glfwGetProcAddress);

		GL_SetupDebugOutput();

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
	}

	GL_API::~GL_API() {
		glDeleteVertexArrays(1, &m_VAO);
	}

	Ref<Shader> GL_API::CreateShader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) {
		return ez::CreateRef<GL_Shader>(shaders);
	}

	Ref<Storage> GL_API::CreateStreamStorage(uint32_t size) {
		Ref<Storage> storage = ez::CreateRef<GL_MappedStreamBuffer>(GL_SHADER_STORAGE_BUFFER, size); 
		m_StreamStorages.push_back(storage);
		return storage;
	}

	Ref<TextureArray> GL_API::CreateTextureArray(uint32_t width, uint32_t height, Format format, Filter filter) {
		return ez::CreateRef<GL_TextureArray>(width, height, format, filter);
	}

	void GL_API::BeginFrame() {
		for (const auto& storage : m_StreamStorages) {
			storage->Lock();
		}
	}

	void GL_API::EndFrame() {
		for (const auto& storage : m_StreamStorages) {
			storage->Unlock();
		}
	}

	void GL_API::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void GL_API::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void GL_API::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GL_API::DrawArrays(DrawMode mode, uint32_t count, uint32_t offset) {
		GLenum glMode = GL_INVALID_ENUM;

		switch (mode) {
			case DrawMode::TRIANGLES:		
				glMode = GL_TRIANGLES; 
				break;

			case DrawMode::TRIANGLE_STRIP:	
				glMode = GL_TRIANGLE_STRIP; 
				break;

			default: 
				EZ_CORE_ERROR("Unknown DrawMode"); 
				break;
		}

		glDrawArrays(glMode, offset, count);
	}
}
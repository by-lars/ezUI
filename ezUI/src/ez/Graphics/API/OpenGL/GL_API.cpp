#include "ez/Core/Base.hpp"
#include "ez/Graphics/API/OpenGL/GL_API.hpp"
#include "ez/Graphics/API/OpenGL/GL_Shader.hpp"
#include "ez/Graphics/API/OpenGL/GL_MappedStreamBuffer.hpp"
#include "ez/Graphics/API/OpenGL/GL_TextureArray.hpp"
#include "ez/Graphics/API/OpenGL/GL_DebugOutput.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.inl>
#include <GLFW/glfw3.h>

namespace ez::gfx {
	GL_API::GL_API() {
		gladLoadGL(glfwGetProcAddress);

        gl_setup_debug_output();

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
	}

	GL_API::~GL_API() {
		glDeleteVertexArrays(1, &m_vao);
	}

	Ref<Shader> GL_API::create_shader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) {
		return ez::create_ref<GL_Shader>(shaders);
	}

	Ref<StreamStorage> GL_API::create_stream_storage(uint32_t size, uint32_t stride) {
		return ez::create_ref<GL_MappedStreamBuffer>(GL_SHADER_STORAGE_BUFFER, size, stride);
	}

	Ref<TextureArray> GL_API::create_texture_array(uint32_t width, uint32_t height, Format format, Filter filter) {
		return ez::create_ref<GL_TextureArray>(width, height, format, filter);
	}


	void GL_API::set_clear_color(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void GL_API::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void GL_API::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GL_API::draw_arrays(DrawMode mode, uint32_t count, uint32_t offset) {
		GLenum glMode = GL_INVALID_ENUM;

		switch (mode) {
			case DrawMode::TRIANGLES: glMode = GL_TRIANGLES; break;
			case DrawMode::TRIANGLE_STRIP: glMode = GL_TRIANGLE_STRIP; break;

			default: 
				EZ_CORE_FATAL_ERROR("Unknown DrawMode");
		}

		glDrawArrays(glMode, offset, count);
	}
}
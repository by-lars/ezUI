#include "ez/Core/Base.hpp"
#include "ez/Graphics/API/OpenGL/GL_Shader.hpp"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace ez::gfx {
	static void compile_shader(GLint shader, const std::string& source) {
		const GLchar* glSource = source.c_str();
		glShaderSource(shader, 1, &glSource, 0);
		glCompileShader(shader);

		GLint success = false;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success == false) {
			char log[4096];
			glGetShaderInfoLog(shader, sizeof(log), 0, log);
			EZ_CORE_FATAL_ERROR("Failed to compile shader: \n", log);
		}
	}

	static GLenum shader_type_to_native(Shader::Type type) {
		switch (type) {
		    case Shader::Type::VERTEX:      return GL_VERTEX_SHADER;
		    case Shader::Type::FRAGMENT:    return GL_FRAGMENT_SHADER;

            default:
                EZ_CORE_FATAL_ERROR("Unknown Enum");
		}
	}

	GL_Shader::GL_Shader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) {
        m_program = glCreateProgram();
		
		for (const auto& shader : shaders) {
			GLuint id = glCreateShader(shader_type_to_native(shader.first));
            compile_shader(id, shader.second);

			glAttachShader(m_program, id);
		}

		glLinkProgram(m_program);

		GLint did_link = false;
		glGetProgramiv(m_program, GL_LINK_STATUS, &did_link);

		if (did_link == false) {
			char log[4096];
			glGetProgramInfoLog(m_program, sizeof(log), 0, log);
			EZ_CORE_FATAL_ERROR("Failed to link shader: \n", log);
		}
	}

	GL_Shader::~GL_Shader() {
		glDeleteProgram(m_program);
	}

	void GL_Shader::bind() {
		glUseProgram(m_program);
	}

	void GL_Shader::set(const std::string& location, const glm::mat4& matrix) {
		glUseProgram(m_program);
		glUniformMatrix4fv(glGetUniformLocation(m_program, location.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GL_Shader::set(const std::string& location, float value) {
		glUseProgram(m_program);
		glUniform1f(glGetUniformLocation(m_program, location.c_str()), value);
	}

	void GL_Shader::set(const std::string& location, int value) {
		glUseProgram(m_program);
		glUniform1i(glGetUniformLocation(m_program, location.c_str()), value);
	}
}
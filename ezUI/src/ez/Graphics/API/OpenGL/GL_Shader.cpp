#include "ez/Core/Base.hpp"
#include "ez/Graphics/API/OpenGL/GL_Shader.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace ez::gfx {
	static void CompileShader(GLint shader, const std::string& source) {
		const GLchar* glSource = source.c_str();
		glShaderSource(shader, 1, &glSource, 0);
		glCompileShader(shader);

		EZ_CORE_DEBUG("Shader Source (", shader, "): ",  source);

		GLint success = false;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success == false) {
			char log[4096];
			glGetShaderInfoLog(shader, sizeof(log), 0, log);
			EZ_CORE_FATAL_ERROR("Failed to compile shader: \n", log);
		}
	}

	static GLenum ShaderTypeToNative(Shader::Type type) {
		switch (type) {
		case Shader::Type::VERTEX:
				return GL_VERTEX_SHADER;

		case Shader::Type::FRAGMENT:
			return GL_FRAGMENT_SHADER;

		default: 
			EZ_CORE_FATAL_ERROR("Unknown Enum"); 
			return GL_INVALID_ENUM;
		}
	}

	GL_Shader::GL_Shader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) {
		m_Program = glCreateProgram();
		
		for (const auto& shader : shaders) {
			GLuint id = glCreateShader(ShaderTypeToNative(shader.first));
			CompileShader(id, shader.second);

			glAttachShader(m_Program, id);
		}

		glLinkProgram(m_Program);

		GLint didLink = false;
		glGetProgramiv(m_Program, GL_LINK_STATUS, &didLink);

		if (didLink == false) {
			char log[4096];
			glGetProgramInfoLog(m_Program, sizeof(log), 0, log);
			EZ_CORE_FATAL_ERROR("Failed to link shader: \n", log);
		}
	}

	GL_Shader::~GL_Shader() {
		glDeleteProgram(m_Program);
	}

	void GL_Shader::Bind() {
		glUseProgram(m_Program);
	}

	void GL_Shader::Set(const std::string& location, const glm::mat4& matrix) {
		glUseProgram(m_Program);
		glUniformMatrix4fv(glGetUniformLocation(m_Program, location.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GL_Shader::Set(const std::string& location, float value) {
		glUseProgram(m_Program);
		glUniform1f(glGetUniformLocation(m_Program, location.c_str()), value);
	}

	void GL_Shader::Set(const std::string& location, int value) {
		glUseProgram(m_Program);
		glUniform1i(glGetUniformLocation(m_Program, location.c_str()), value);
	}
}
#include "Core/Base.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.inl>
#include <GLFW/glfw3.h>

namespace ez {
	static void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:ez::Logger::Log(LogSender::API_OGL, LogLevel::ERROR, message); break;
		case GL_DEBUG_SEVERITY_MEDIUM: ez::Logger::Log(LogSender::API_OGL, LogLevel::WARNING, message); break;
		case GL_DEBUG_SEVERITY_LOW: ez::Logger::Log(LogSender::API_OGL, LogLevel::LOG, message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: ez::Logger::Log(LogSender::API_OGL, LogLevel::LOG, message); break;
		}
	}

	OpenGLAPI::OpenGLAPI() {
		gladLoadGL(glfwGetProcAddress);

		const GLubyte* oglVersion = glGetString(GL_VERSION);
		const GLubyte* oglRenderer = glGetString(GL_RENDERER);
		const GLubyte* oglVendor = glGetString(GL_VENDOR);
		EZ_CORE_LOG("Running ", oglVersion, " on ", oglRenderer, " - ", oglVendor);

		#ifdef EZ_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		#endif

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
	}

	OpenGLAPI::~OpenGLAPI() {
		for (const GLShader& shader : m_Shaders) {
			EZ_CORE_DEBUG_DEALLOC("Deleting Shader Id=", shader.Id);
			glDeleteProgram(shader.Id);
		}

		for (const GLBuffer& buffer : m_Buffers) {
			EZ_CORE_DEBUG_DEALLOC("Deleting Buffer Id=", buffer.Id);
			glDeleteBuffers(1, &buffer.Id);
		}

		glDeleteVertexArrays(1, &m_VAO);
	}
	
	/*
	 * SHADER
	 */
	static void CompileShader(GLint shader, const GLchar* source) {
		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		GLint success = false;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success == false) {
			char log[2048];
			glGetShaderInfoLog(shader, sizeof(log), 0, log);
			EZ_CORE_ERROR("Failed to compile shader: ", shader, "\n", log);
		}
	}

	HShader OpenGLAPI::CreateShader(const std::string& vertexCode, const std::string& fragmentCode) {
		GLuint program  = glCreateProgram();
		GLuint vertex   = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		EZ_CORE_DEBUG_ALLOC("Compiling Shader Id= ", program);
	
		CompileShader(vertex, vertexCode.c_str());
		CompileShader(fragment, fragmentCode.c_str());

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		GLint didLink = false;
		glGetProgramiv(program, GL_LINK_STATUS, &didLink);

		if (didLink == false) {
			char log[2048];
			glGetProgramInfoLog(program, sizeof(log), 0, log);
			EZ_CORE_ERROR("Failed to link shader: ", program, "\n", log);
		}

		m_Shaders.push_back(GLShader {program});

		return HShader(m_Shaders.size() - 1);
	}

	void OpenGLAPI::SetShaderUniform(HShader hShader, const std::string& location, const glm::mat4& matrix) {
		GLShader shader = m_Shaders[(int)hShader];
		glUseProgram(shader.Id);
		glUniformMatrix4fv(glGetUniformLocation(shader.Id, location.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLAPI::SetShaderUniform(HShader hShader, const std::string& location, float value) {
		GLShader shader = m_Shaders[(int)hShader];
		glUseProgram(shader.Id);
		glUniform1f(glGetUniformLocation(shader.Id, location.c_str()), value);
	}

	void OpenGLAPI::BindShader(HShader hShader) {
		GLShader shader = m_Shaders[(int)hShader];
		glUseProgram(shader.Id);
	}

	/*
	 * BUFFER
	 */
	HBuffer OpenGLAPI::CreateBuffer(uint32_t size, BufferType type, void* data) {
		EZ_CORE_DEBUG_ALLOC("Creating buffer of size: ", (float)size / 1048576.0f, "mb");

		GLuint buffer = 0;
		glGenBuffers(1, &buffer);
	
		GLenum bufferType;
		switch (type) {
			case BufferType::ARRAY: bufferType = GL_ARRAY_BUFFER; break;
			case BufferType::SHADER_STORAGE: bufferType = GL_SHADER_STORAGE_BUFFER; break;
			default: EZ_CORE_ERROR("Unknown buffer type enum"); break;
		}

		GLenum flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT;

		glBindBuffer(bufferType, buffer);
		glBufferStorage(bufferType, size, data, flags);
		glBindBufferBase(bufferType, 0, buffer);

		m_Buffers.push_back(GLBuffer {
			.Id = buffer,
			.Type = bufferType,
			.SizeInBytes = size
		});

		return HBuffer(m_Buffers.size() - 1);
	}

	void* OpenGLAPI::GetBufferPointer(HBuffer hBuffer) {
		GLBuffer buffer = m_Buffers[(int)hBuffer];
		glBindBuffer(buffer.Type, buffer.Id);
		return glMapBufferRange(buffer.Type, 0, buffer.SizeInBytes, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	}
	void OpenGLAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void OpenGLAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static GLenum DrawModeToGLEnum(DrawMode mode) {
		switch (mode) {
			case DrawMode::TRIANGLES: return GL_TRIANGLES; break;
			case DrawMode::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP; break;
			default: EZ_CORE_ERROR("Unknown DrawMode"); break;
		}

		return GL_INVALID_ENUM;
	}

	void OpenGLAPI::DrawArrays(DrawMode mode, uint32_t count, uint32_t offset) {
		GLenum glMode = DrawModeToGLEnum(mode);
		glDrawArrays(glMode, offset, count);
	}
}
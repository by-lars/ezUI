#include "ez/Graphics/API/OpenGL/GL_DebugOutput.h"
#include "ez/Core/Base.hpp"

void ez::gfx::GL_SetupDebugOutput() {
	const GLubyte* oglVersion = glGetString(GL_VERSION);
	const GLubyte* oglRenderer = glGetString(GL_RENDERER);
	const GLubyte* oglVendor = glGetString(GL_VENDOR);
	EZ_CORE_LOG("Running ", oglVersion, " on ", oglRenderer, " - ", oglVendor);

	#if EZ_BUILD_DEBUG_MODE
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(ez::gfx::GL_DebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	#endif
}

void ez::gfx::GL_DebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		ez::Logger::log(ez::Logger::Sender::API_OGL, ez::Logger::Level::LOG, "[NOTIFICTION] ", message);
		break;

	case GL_DEBUG_SEVERITY_LOW:
		ez::Logger::log(ez::Logger::Sender::API_OGL, ez::Logger::Level::LOG, "[LOW] ", message);
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		ez::Logger::log(ez::Logger::Sender::API_OGL, ez::Logger::Level::WARNING, "[MEDIUM] ", message);
		break;

	case GL_DEBUG_SEVERITY_HIGH:
		ez::Logger::log(ez::Logger::Sender::API_OGL, ez::Logger::Level::ERROR, "[ERROR] ", message);
		break;
	}
}

#include "ez/Graphics/API/OpenGL/GL_DebugOutput.hpp"
#include "ez/Core/Base.hpp"

void ez::gfx::gl_setup_debug_output() {
	const GLubyte* oglVersion = glGetString(GL_VERSION);
	const GLubyte* oglRenderer = glGetString(GL_RENDERER);
	const GLubyte* oglVendor = glGetString(GL_VENDOR);
	EZ_CORE_LOG("Running ", oglVersion, " on ", oglRenderer, " - ", oglVendor);

	#if EZ_BUILD_DEBUG_MODE
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(ez::gfx::gl_debug_output, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	#endif
}

void ez::gfx::gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
	switch (severity) {
	    case GL_DEBUG_SEVERITY_NOTIFICATION:
	    	ez::Logger::log("GL", ez::Logger::Level::LOG, "Notification: ", message);
	    	break;

	    case GL_DEBUG_SEVERITY_LOW:
	    	ez::Logger::log("GL", ez::Logger::Level::LOG, message);
	    	break;

	    case GL_DEBUG_SEVERITY_MEDIUM:
	    	ez::Logger::log("GL", ez::Logger::Level::WARNING,  message);
	    	break;

	    case GL_DEBUG_SEVERITY_HIGH:
	    	ez::Logger::log("GL", ez::Logger::Level::ERROR, message);
	    	break;
            
        default:
            ez::Logger::log("GL", ez::Logger::Level::WARNING, message);
	}
}

#pragma once
#include <glad/gl.h>

namespace ez::gfx {
	void GL_SetupDebugOutput();
	void GL_DebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
}
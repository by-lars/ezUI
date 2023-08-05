#pragma once
#include <glad/gl.h>

namespace ez::gfx {
	void gl_setup_debug_output();
	void gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
}
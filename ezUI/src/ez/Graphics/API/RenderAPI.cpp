#include "ez/Core/Base.hpp"
#include "ez/Graphics/API/RenderAPI.hpp"

#include "ez/Graphics/API/OpenGL/GL_API.hpp"

namespace ez::gfx {
	std::shared_ptr<RenderAPI> RenderAPI::create(ez::gfx::API api) {
		switch (api) {
			case gfx::API::NONE:    EZ_CORE_FATAL_ERROR("RenderAPI None is not supported.");
			case gfx::API::OPENGL:  return ez::create_ref<GL_API>();
			default:                EZ_CORE_FATAL_ERROR("Unknown RenderAPI.");
		}
	}
}
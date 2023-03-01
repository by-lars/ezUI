#include "ez/Core/Base.h"
#include "ez/Graphics/API/RenderAPI.h"

#include "ez/Graphics/API/OpenGL/GL_API.h"

namespace ez::gfx {
	std::shared_ptr<RenderAPI> RenderAPI::Create(ez::gfx::API api) {
		switch (api) {
			case gfx::API::NONE: 
				EZ_CORE_FATAL_ERROR("RenderAPI None is not supported."); 
				return nullptr;

			case gfx::API::OPENGL: 
				return ez::CreateRef<GL_API>();  

			default: 
				EZ_CORE_FATAL_ERROR("Unknown RenderAPI.");
				return nullptr;
		}
	}
}
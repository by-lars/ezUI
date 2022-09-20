#include "Core/Base.h"
#include "Graphics/RenderAPI.h"

#include "Platform/OpenGL/OpenGLAPI.h"

namespace ez::gfx {
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OPENGL;

	std::shared_ptr<RenderAPI> RenderAPI::Create() {
		switch (s_API) {
		case RenderAPI::API::NONE: EZ_CORE_ERROR("RenderAPI None is not supported."); break;
		case RenderAPI::API::OPENGL: return std::make_shared<OpenGLAPI>();  break;
		default: EZ_CORE_ERROR("Unknown RenderAPI."); break;
		}
	}

	RenderAPI::API RenderAPI::GetAPI() {
		return s_API;
	}
}
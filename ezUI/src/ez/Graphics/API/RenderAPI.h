#pragma once
#include "ez/Core/Base.h"
#include "ez/Graphics/API/DeviceResources.h"

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <initializer_list>

namespace ez::gfx {
	enum class API {
		NONE = 0,
		OPENGL
	};

	class RenderAPI {
	public:
		virtual ~RenderAPI() = default;

		static std::shared_ptr<RenderAPI> Create(ez::gfx::API api);

		virtual Ref<Shader>			CreateShader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) = 0;
		virtual Ref<StreamStorage>	CreateStreamStorage(uint32_t count, uint32_t stride) = 0;
		virtual Ref<TextureArray>	CreateTextureArray(uint32_t width, uint32_t height, Format format, Filter filter) = 0;

		virtual void Clear() = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DrawArrays(DrawMode mode, uint32_t count, uint32_t offset) = 0;
	};
}
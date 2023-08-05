#pragma once
#include "ez/Core/Base.hpp"
#include "ez/Graphics/API/DeviceResources.hpp"

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

		static std::shared_ptr<RenderAPI> create(ez::gfx::API api);

		virtual Ref<Shader>			create_shader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) = 0;
		virtual Ref<StreamStorage>	create_stream_storage(uint32_t count, uint32_t stride) = 0;
		virtual Ref<TextureArray>	create_texture_array(uint32_t width, uint32_t height, Format format, Filter filter) = 0;

		virtual void clear() = 0;
		virtual void set_clear_color(float r, float g, float b, float a) = 0;
		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void draw_arrays(DrawMode mode, uint32_t count, uint32_t offset) = 0;
	};
}
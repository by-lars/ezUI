#pragma once
#include "ez/Graphics/API/RenderAPI.hpp"

namespace ez::gfx {
	class GL_API : public RenderAPI {
	public:
		GL_API();
		~GL_API() override;

		Ref<Shader>			create_shader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) override;
		Ref<StreamStorage>	create_stream_storage(uint32_t size, uint32_t stride) override;
		Ref<TextureArray>	create_texture_array(uint32_t width, uint32_t height, Format format, Filter filter) override;

		void clear() override;
		void set_clear_color(float r, float g, float b, float a) override;
		void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void draw_arrays(DrawMode mode, uint32_t count, uint32_t offset) override;

	private:
		uint32_t m_vao;
	};
}
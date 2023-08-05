#pragma once
#include "ez/Graphics/API/RenderAPI.hpp"

namespace ez::gfx {
	class GL_API : public RenderAPI {
	public:
		GL_API();
		~GL_API();

		virtual Ref<Shader>			CreateShader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) override;
		virtual Ref<StreamStorage>	CreateStreamStorage(uint32_t size, uint32_t stride) override;
		virtual Ref<TextureArray>	CreateTextureArray(uint32_t width, uint32_t height, Format format, Filter filter) override;

		virtual void Clear() override;
		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawArrays(DrawMode mode, uint32_t count, uint32_t offset) override;

	private:
		uint32_t m_VAO;
	};
}
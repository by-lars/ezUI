#pragma once
#include "ez/Graphics/API/DeviceResources.hpp"
#include <glad/gl.h>

namespace ez::gfx {
	class GL_TextureArray : public TextureArray {
	public:
		GL_TextureArray(uint32_t width, uint32_t height, Format format, Filter filter);
		~GL_TextureArray();

		void Bind() override;
		Layer PushBack(void* data) override;
		void Erase(Layer layer) override;
		virtual void BindToSlot(uint32_t slot) override;
		uint32_t GetMaxLayers() override;

	private:
		GLuint m_Texture;
		GLuint m_Width;
		GLuint m_Height;
		GLenum m_Format;
		GLint m_MaxLayers;
		GLuint m_NextLayer;

		std::queue<Layer> m_FreeLayers;
	};
}
#pragma once
#include "ez/Graphics/API/Enums.hpp"
#include "ez/Graphics/API/TextureArray.hpp"

#include <glad/gl.h>

namespace ez::gfx {
	class GL_TextureArray : public TextureArray {
	public:
		GL_TextureArray(uint32_t width, uint32_t height, Format format, Filter filter);
		~GL_TextureArray() override;

		void bind() override;
		Layer push_back(void* data) override;
		void erase(Layer layer) override;
		void bind_to_slot(uint32_t slot) override;
		uint32_t get_max_layers() override;

	private:
		GLuint m_texture;
		GLuint m_width;
		GLuint m_height;
		GLenum m_format;
		GLint m_max_layers;
		GLuint m_next_layer;

		std::queue<Layer> m_free_layers;
	};
}
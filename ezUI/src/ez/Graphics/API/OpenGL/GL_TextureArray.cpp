#include "ez/Graphics/API/OpenGL/GL_TextureArray.hpp"
#include "ez/Core/Base.hpp"

#include <glad/gl.h>

namespace ez::gfx {
	static GLenum texture_format_to_native(Format format) {
		switch (format) {
            case Format::RGB:   return GL_RGB;
		    case Format::RGBA:  return GL_RGBA;

		default:
            EZ_CORE_FATAL_ERROR("Unknown Texture Format");
		}
	}

	static GLenum texture_format_to_internal_native(Format format) {
		switch (format) {
		    case Format::RGB:   return GL_RGB8;
		    case Format::RGBA:  return GL_RGBA8;

		default:
            EZ_CORE_FATAL_ERROR("Unknown Texture Format");
		}
	}

	static GLenum texture_filter_to_native(Filter filter) {
		switch (filter) {
		    case Filter::LINEAR:    return GL_LINEAR;
            case Filter::NEAREST:   return GL_NEAREST;

		default:
            EZ_CORE_FATAL_ERROR("Unknown Texture Format");
		}
	}

	GL_TextureArray::GL_TextureArray(uint32_t width, uint32_t height, Format format, Filter filter) {
		GLenum internalNativeFormat = texture_format_to_internal_native(format);
		GLenum nativeFormat = texture_format_to_native(format);
		GLenum nativeFilter = texture_filter_to_native(filter);

        m_width = width;
        m_height = height;
        m_format = nativeFormat;
        m_max_layers = 0;
        m_next_layer = 0;

		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &m_max_layers);

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalNativeFormat, width, height, m_max_layers);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, nativeFilter);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, nativeFilter);
	}

	GL_TextureArray::~GL_TextureArray() {
		glDeleteTextures(1, &m_texture);
	}

	void GL_TextureArray::bind() {
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	}

	uint32_t GL_TextureArray::get_max_layers() {
		return m_max_layers;
	}

	void GL_TextureArray::bind_to_slot(uint32_t slot) {
		EZ_CORE_ASSERT(slot > 0, "Slot must be grater than 0");
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
		glActiveTexture(GL_TEXTURE0);
	}

	Layer GL_TextureArray::push_back(void* data) {
		Layer layer;
		if (m_free_layers.empty()) {
			layer = Layer(m_next_layer);
			m_next_layer++;
		}
		else {
			layer = m_free_layers.front();
			m_free_layers.pop();
		}

		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, (GLint)layer, m_width, m_height, 1, m_format, GL_UNSIGNED_BYTE, data);
		return layer;	
	}

	void GL_TextureArray::erase(Layer layer) {
		m_free_layers.push(layer);
	}

	//HTexture OpenGLAPI::CreateTexture(uint32_t width, uint32_t height, TextureFormat format, TextureFilter filter) {
	//	GLenum internalNativeFormat = texture_format_to_internal_native(format);
	//	GLenum nativeFormat = texture_format_to_native(format);
	//	GLenum nativeFilter = texture_filter_to_native(filter);

	//	GLuint tex;
	//	glGenTextures(1, &tex);
	//	glBindTexture(GL_TEXTURE_2D, tex);
	//	glTexStorage2D(GL_TEXTURE_2D, 1, internalNativeFormat, width, height);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nativeFilter);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nativeFilter);

	//	m_Textures.push_back(GLTexture{
	//		.Id = tex,
	//		.Format = nativeFormat,
	//		.width = width,
	//		.height = height
	//		});

	//	return HTexture(m_Textures.size() - 1);
	//}


	//void OpenGLAPI::UploadTexture(HTexture hTexture, void* data) {
	//	GLTexture tex = m_Textures[(int)hTexture];
	//	glBindTexture(GL_TEXTURE_2D, tex.Id);
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width, tex.height, tex.Format, GL_UNSIGNED_BYTE, data);
	//}

	//void OpenGLAPI::UploadTexture(HTextureArray hTexture, uint32_t layer, void* data) {
	//	GLTextureArray tex = m_TextureArrays[(int)hTexture];
	//	glBindTexture(GL_TEXTURE_2D_ARRAY, tex.Id);
	//	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, tex.width, tex.height, 1, tex.Format, GL_UNSIGNED_BYTE, data);
	//}
}
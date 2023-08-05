#include "ez/Graphics/API/OpenGL/GL_TextureArray.h"
#include "ez/Core/Base.hpp"

#include <glad/gl.h>

namespace ez::gfx {
	static GLenum TextureFormatToNative(Format format) {
		switch (format) {
		case Format::RGB:	
			return GL_RGB;		
		
		case Format::RGBA:	
			return GL_RGBA;		

		default: 
			EZ_CORE_ERROR("Unknown Texture Format");
			return GL_INVALID_ENUM;
		}
	}

	static GLenum TextureFormatToInternalNative(Format format) {
		switch (format) {
		case Format::RGB:	
			return GL_RGB8;

		case Format::RGBA:	
			return GL_RGBA8;

		default:
			EZ_CORE_ERROR("Unknown Texture Format");
			return GL_INVALID_ENUM;
		}
	}

	static GLenum TextureFilterToNative(Filter filter) {
		switch (filter) {
		case Filter::LINEAR: 
			return GL_LINEAR;	
		
		case Filter::NEAREST: 
			return GL_NEAREST;

		default: 
			EZ_CORE_ERROR("Unknown Texture Format");
			return GL_INVALID_ENUM;
		}
	}

	GL_TextureArray::GL_TextureArray(uint32_t width, uint32_t height, Format format, Filter filter) {
		GLenum internalNativeFormat = TextureFormatToInternalNative(format);
		GLenum nativeFormat = TextureFormatToNative(format);
		GLenum nativeFilter = TextureFilterToNative(filter);

		m_Width = width;
		m_Height = height;
		m_Format = nativeFormat;
		m_MaxLayers = 0;
		m_NextLayer = 0;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &m_MaxLayers);

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_Texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalNativeFormat, width, height, m_MaxLayers);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, nativeFilter);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, nativeFilter);
	}

	GL_TextureArray::~GL_TextureArray() {
		glDeleteTextures(1, &m_Texture);
	}

	void GL_TextureArray::Bind() {
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_Texture);
	}

	uint32_t GL_TextureArray::GetMaxLayers() {
		return m_MaxLayers;
	}

	void GL_TextureArray::BindToSlot(uint32_t slot) {
		EZ_CORE_ASSERT(slot > 0, "Slot must be grater than 0");
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_Texture);
		glActiveTexture(GL_TEXTURE0);
	}

	Layer GL_TextureArray::PushBack(void* data) {
		Layer layer;
		if (m_FreeLayers.empty()) {
			layer = Layer(m_NextLayer);
			m_NextLayer++;
		}
		else {
			layer = m_FreeLayers.front();
			m_FreeLayers.pop();
		}

		glBindTexture(GL_TEXTURE_2D_ARRAY, m_Texture);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, (GLint)layer, m_Width, m_Height, 1, m_Format, GL_UNSIGNED_BYTE, data);
		return layer;	
	}

	void GL_TextureArray::Erase(Layer layer) {
		m_FreeLayers.push(layer);
	}

	//HTexture OpenGLAPI::CreateTexture(uint32_t width, uint32_t height, TextureFormat format, TextureFilter filter) {
	//	GLenum internalNativeFormat = TextureFormatToInternalNative(format);
	//	GLenum nativeFormat = TextureFormatToNative(format);
	//	GLenum nativeFilter = TextureFilterToNative(filter);

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
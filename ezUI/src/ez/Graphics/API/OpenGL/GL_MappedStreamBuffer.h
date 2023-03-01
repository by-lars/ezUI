#pragma once
#include "ez/Graphics/API/DeviceResources.h"
#include <glad/gl.h>
#include <array>

namespace ez::gfx {
	class GL_MappedStreamBuffer : public Storage {
	public:
		GL_MappedStreamBuffer(GLenum bufferType, uint32_t size);
		~GL_MappedStreamBuffer();

		void Upload(uint32_t nBytes, uint32_t offset, void* data) override;
		void Bind() override;
		
		void Lock() override;
		void Unlock() override;

	private:
		void Grow();

		uint8_t* m_Data;
		std::array<GLsync, 3> m_Fences;
		uint8_t m_Index;
		uint32_t m_Offset;
		uint32_t m_Id;
		uint32_t m_MaxSize;
		GLenum m_Type;
	};
}
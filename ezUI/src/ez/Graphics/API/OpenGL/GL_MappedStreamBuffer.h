#pragma once
#include "ez/Graphics/API/DeviceResources.h"
#include <glad/gl.h>
#include <array>

namespace ez::gfx {
	class GL_MappedStreamBuffer : public StreamStorage {
	public:
		GL_MappedStreamBuffer(GLenum bufferType, uint32_t count, uint32_t stride);
		~GL_MappedStreamBuffer();

		void PushBack(void* data) override;
		uint32_t Count() override;
		uint32_t Offset() override;

		void BeginFrame() override;
		void EndFrame() override;

	private:
		uint32_t m_Id;
		uint8_t* m_Data;
		GLenum m_Type;

		std::array<GLsync, 3> m_Fences;
		uint8_t m_CurrentFence;

		uint32_t m_Offset;
		uint32_t m_MaxCount;
		uint32_t m_Stride;
		uint32_t m_Count;
	};
}
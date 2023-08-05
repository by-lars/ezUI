#pragma once
#include "ez/Graphics/API/DeviceResources.hpp"
#include <glad/gl.h>
#include <array>

namespace ez::gfx {
	class GL_MappedStreamBuffer : public StreamStorage {
	public:
		GL_MappedStreamBuffer(GLenum bufferType, uint32_t count, uint32_t stride);
		~GL_MappedStreamBuffer() override;

		void push_back(void* data) override;
		uint32_t count() override;
		uint32_t offset() override;

		void begin_frame() override;
		void end_frame() override;

	private:
		uint32_t m_id;
		uint8_t* m_data;
		GLenum m_type;

		std::array<GLsync, 3> m_fences;
		uint8_t m_current_fence;

		uint32_t m_offset;
		uint32_t m_max_count;
		uint32_t m_stride;
		uint32_t m_count;
	};
}
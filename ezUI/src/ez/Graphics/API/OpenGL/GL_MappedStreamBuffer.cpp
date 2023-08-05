#include "ez/Graphics/API/OpenGL/GL_MappedStreamBuffer.hpp"

#include <glad/gl.h>
#include <cstring>

namespace ez::gfx {
	GL_MappedStreamBuffer::GL_MappedStreamBuffer(GLenum bufferType, uint32_t count, uint32_t stride) {
        m_data = nullptr;
        m_type = bufferType;
        m_stride = stride;
        m_max_count = count;
        m_offset = 0;
        m_current_fence = 0;
        m_id = 0;
        m_fences = {0 };
        m_count = 0;

		uint32_t bufferSize = count * 3 * m_stride;
		
		glGenBuffers(1, &m_id);

		GLenum flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT;

		glBindBuffer(bufferType, m_id);
		glBufferStorage(bufferType, bufferSize, 0, flags);
		glBindBufferBase(bufferType, 0, m_id);
        m_data = (uint8_t*)glMapBufferRange(m_type, 0, bufferSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

		if (m_data == nullptr) {
			EZ_CORE_ERROR("Could not get pointer of OpenGL Buffer");
		}
	}

	GL_MappedStreamBuffer::~GL_MappedStreamBuffer() {

	}

	void GL_MappedStreamBuffer::push_back(void* data) {
		void* location = m_data + m_offset * m_stride + m_count * m_stride;
		std::memcpy(location, data, m_stride);
		m_count++;
	}

	uint32_t GL_MappedStreamBuffer::count() {
		return m_count;
	}

	uint32_t GL_MappedStreamBuffer::offset() {
		return m_offset;
	}


	void GL_MappedStreamBuffer::begin_frame() {
		GLsync sync = m_fences[m_current_fence];

		if (sync) {
			while (true) {
				GLenum wait = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
				if (wait == GL_ALREADY_SIGNALED || wait == GL_CONDITION_SATISFIED) {
					return;
				}
			}
		}
	}

	void GL_MappedStreamBuffer::end_frame() {
		GLsync sync = m_fences[m_current_fence];

		if (sync) {
			glDeleteSync(sync);
		}

        m_fences[m_current_fence] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

        m_current_fence = (m_current_fence + 1) % 3;
        m_offset = m_current_fence * m_max_count;
        m_count = 0;
	}
}
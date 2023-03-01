#include "ez/Graphics/API/OpenGL/GL_MappedStreamBuffer.h"

#include <glad/gl.h>

namespace ez::gfx {
	GL_MappedStreamBuffer::GL_MappedStreamBuffer(GLenum bufferType, uint32_t count, uint32_t stride) {
		m_Data = nullptr;
		m_Type = bufferType;
		m_Stride = stride;
		m_MaxCount = count;
		m_Offset = 0;
		m_CurrentFence = 0;
		m_Id = 0;
		m_Fences = { 0 };
		m_Count = 0;

		uint32_t bufferSize = count * 3 * m_Stride;
		
		glGenBuffers(1, &m_Id);

		GLenum flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT;

		glBindBuffer(bufferType, m_Id);
		glBufferStorage(bufferType, bufferSize, 0, flags);
		glBindBufferBase(bufferType, 0, m_Id);
		m_Data = (uint8_t*)glMapBufferRange(m_Type, 0, bufferSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

		if (m_Data == nullptr) {
			EZ_CORE_ERROR("Could not get pointer of OpenGL Buffer");
		}
	}

	GL_MappedStreamBuffer::~GL_MappedStreamBuffer() {

	}

	void GL_MappedStreamBuffer::PushBack(void* data) {
		void* location = m_Data + m_Offset * m_Stride + m_Count * m_Stride;

		memcpy(location, data, m_Stride);
		m_Count++;
	}

	uint32_t GL_MappedStreamBuffer::Count() {
		return m_Count;
	}

	uint32_t GL_MappedStreamBuffer::Offset() {
		return m_Offset;
	}


	void GL_MappedStreamBuffer::BeginFrame() {
		GLsync sync = m_Fences[m_CurrentFence];

		if (sync) {
			while (true) {
				GLenum wait = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
				if (wait == GL_ALREADY_SIGNALED || wait == GL_CONDITION_SATISFIED) {
					return;
				}
			}
		}
	}

	void GL_MappedStreamBuffer::EndFrame() {
		GLsync sync = m_Fences[m_CurrentFence];

		if (sync) {
			glDeleteSync(sync);
		}

		m_Fences[m_CurrentFence] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

		m_CurrentFence = (m_CurrentFence + 1) % 3;
		m_Offset = m_CurrentFence * m_MaxCount;
		m_Count = 0;
	}
}
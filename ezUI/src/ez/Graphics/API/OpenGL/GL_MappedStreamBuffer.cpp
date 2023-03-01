#include "ez/Graphics/API/OpenGL/GL_MappedStreamBuffer.h"

#include <glad/gl.h>

namespace ez::gfx {
	GL_MappedStreamBuffer::GL_MappedStreamBuffer(GLenum bufferType, uint32_t size)
	: m_MaxSize(size * 3), m_Type(bufferType), m_Data(nullptr) {
		glGenBuffers(1, &m_Id);

		GLenum flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT;

		glBindBuffer(bufferType, m_Id);
		glBufferStorage(bufferType, m_MaxSize, 0, flags);
		glBindBufferBase(bufferType, 0, m_Id);
		m_Data = (uint8_t*)glMapBufferRange(m_Type, 0, m_MaxSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	}

	GL_MappedStreamBuffer::~GL_MappedStreamBuffer() {

	}

	void GL_MappedStreamBuffer::Upload(uint32_t nBytes, uint32_t offset, void* data) {
		memcpy((void*)m_Data[offset + m_Offset], data, nBytes);
	}

	void GL_MappedStreamBuffer::Bind() {

	}

	void GL_MappedStreamBuffer::Grow() {

	}

	void GL_MappedStreamBuffer::Lock() {
		GLsync sync = m_Fences[m_Index];

		if (sync) {
			while (true) {
				GLenum wait = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
				if (wait == GL_ALREADY_SIGNALED || wait == GL_CONDITION_SATISFIED) {
					return;
				}
			}
		}
	}

	void GL_MappedStreamBuffer::Unlock() {
		GLsync sync = m_Fences[m_Index];

		if (sync) {
			glDeleteSync(sync);
		}

		m_Fences[m_Index] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

		m_Index = (m_Index + 1) % 3;
		m_Offset = m_Index * m_MaxSize;
	}
}
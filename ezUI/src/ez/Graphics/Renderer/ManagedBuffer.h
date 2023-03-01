#pragma once
#include "ez/Graphics/RenderAPI.h"
#include "ez/Graphics/DeviceResources.h"
#include "ez/Core/Base.h"

#include <memory>
#include <queue>
#include <unordered_map>
#include <initializer_list>
#include <array>

#include <glad/gl.h>

namespace ez::gfx {
	template<typename T>
	class ManagedBuffer {
	public:
		static constexpr float GROW_FACTOR = 1.5f;

		static std::unique_ptr<ManagedBuffer<T>> Create(std::shared_ptr<RenderAPI> device, uint32_t maxCount) {
			return std::make_unique<ManagedBuffer<T>>(device, maxCount);
		}

		ManagedBuffer(std::shared_ptr<RenderAPI> device, uint32_t maxCount) : m_MaxCount(maxCount), m_Device(device) {
			m_BufferHandle = device->CreateBuffer(maxCount * sizeof(T) * 3, BufferType::SHADER_STORAGE, 0);
			m_GPUStorage = (T*)device->GetBufferPointer(m_BufferHandle);

			m_Count = 0;
			m_Offset = 0;
			m_Index = 0;
			m_Fences = { 0 };
		}

		void PushBack(const T& data) {
			m_GPUStorage[m_Offset + m_Count] = data;
			m_Count++;
		}

		uint32_t GetOffset() {
			return m_Offset;
		}

		uint32_t GetCount() {
			return m_Count;
		}

		void BeginScene() {
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

		void EndScene() {
			GLsync sync = m_Fences[m_Index];

			if (sync) {
				glDeleteSync(sync);
			}

			m_Fences[m_Index] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

			m_Index = (m_Index + 1) % 3;
			m_Offset = m_Index * m_MaxCount;
			m_Count = 0;
		}

	private:
		uint32_t m_Offset;
		uint32_t m_Count;
		uint32_t m_MaxCount;
		uint8_t m_Index;

		std::shared_ptr<RenderAPI> m_Device;
		HBuffer m_BufferHandle;
		std::array<GLsync, 3> m_Fences;
		T* m_GPUStorage;
	};
}
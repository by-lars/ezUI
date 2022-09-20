#pragma once
#include "Core/Base.h"
#include <vector>
#include <queue>
#include "StrongHandle.h"


namespace ez {
	template<typename Handle, typename Object>
	class HandleStore {
	public:
		struct Entry {
			Handle Handle;
			Object Object;
			bool IsActive;
		};

		struct Iterator {
			Iterator(std::vector<Entry>* entries, size_t index) : m_Entries(entries), m_Index(index) { }

			Object& operator*() const {
				return (*m_Entries)[m_Index].Object;
			}

			bool operator==(const Iterator& other) const {
				return m_Index == other.m_Index;
			}

			bool operator!=(const Iterator& other) const {
				return m_Index != other.m_Index;
			}

			Iterator& operator++() {
				do {
					m_Index++;
				} while (m_Index < m_Entries->size() && (*m_Entries)[m_Index].IsActive == false);

				return *this;
			}

		private:
			std::vector<Entry>* m_Entries;
			size_t m_Index;
		};

		HandleStore() = default;

		Handle PushBack(const Object& object) {
			if (m_FreeHandles.empty()) {
				Handle handle(m_Objects.size());

				m_Objects.push_back(Entry{
					.Handle = handle,
					.Object = object,
					.IsActive = true
				});

				return handle;
			}

			Handle handle = m_FreeHandles.front();
			m_FreeHandles.pop();
			m_Objects[(int)handle].Object = object;
			m_Objects[(int)handle].Handle = handle;
			m_Objects[(int)handle].IsActive = true;

			return handle;
		}

		Object& Get(Handle handle) {
			EZ_CORE_ASSERT((int)handle < m_Objects.size(), "Invalid Handle");

			Entry& entry = m_Objects[(int)handle];
			EZ_CORE_ASSERT(entry.Handle == handle, "Version numbers do not match");
		
			return entry.Object;
		}

		void Erase(Handle h) {
			EZ_CORE_ASSERT((int)h < m_Objects.size(), "Invalid Handle");

			Entry& entry = m_Objects[(int)h];
			EZ_CORE_ASSERT(entry.Handle == h, "Version numbers do not match");

			entry.Handle.Invalidate();
			entry.IsActive = false;

			m_FreeHandles.push(entry.Handle);
		}

		Iterator begin() {
			size_t firstIndex = 0;
			while (firstIndex < m_Objects.size() && m_Objects[firstIndex].IsActive == false) {
				firstIndex++;
			}

			return Iterator(&m_Objects, firstIndex);
		}

		Iterator end() {
			return Iterator(&m_Objects, m_Objects.size());
		}

	private:
		std::queue<Handle> m_FreeHandles;
		std::vector<Entry> m_Objects;
	};

}
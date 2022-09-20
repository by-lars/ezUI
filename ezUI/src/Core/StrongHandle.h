#pragma once

namespace ez {
	template <class Tag, class T, T DEFAULT_VALUE>
	class Handle {
	public:
		Handle() : m_Value(DEFAULT_VALUE), m_Version(0) {}

		static Handle Invalid() { return Handle(); }

		explicit Handle(T value) : m_Value(value), m_Version(0) {}
		explicit operator T() const { return m_Value; }

		void Invalidate() { m_Version++; }
		T GetVersion() const { return m_Version; }

		friend bool operator==(Handle a, Handle b) { return a.m_Value == b.m_Value && a.m_Version == b.m_Version; }
		friend bool operator!=(Handle a, Handle b) { return a.m_Value != b.m_Value || a.m_Version != b.m_Version; }

	private:
		T m_Value;
		T m_Version;
	};

#define EZ_MAKE_STRONG_HANDLE(name)	namespace detail { struct name##_tag {}; } typedef ez::Handle<detail::name##_tag, int, -1> name;

}
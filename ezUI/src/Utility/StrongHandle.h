#pragma once

namespace ez {
	template <class Tag, class T, T DEFAULT_VALUE>
	class Handle {
	public:
		Handle() : m_Value(DEFAULT_VALUE) {}

		static Handle Invalid() { return Handle(); }

		explicit Handle(T value) : m_Value(value) {}
		explicit operator T() const { return m_Value; }

		friend bool operator==(Handle a, Handle b) { return a.m_Value == b.m_Value; }
		friend bool operator!=(Handle a, Handle b) { return a.m_Value != b.m_Value; }

	private:
		T m_Value;
	};

#define EZ_MAKE_STRONG_HANDLE(name)	namespace detail { struct name##_tag {}; } typedef Handle<detail::name##_tag, int, -1> name;

}
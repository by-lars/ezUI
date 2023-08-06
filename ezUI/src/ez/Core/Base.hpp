#pragma once

#include "ez/Core/Logger.hpp"

#include <string>
#include <iostream>
#include <memory>

namespace ez {
	constexpr int VERSION_MAJOR = 0;
	constexpr int VERSION_MINOR = 1;
	constexpr int VERSION_PATCH = 0;

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> create_scope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> create_ref(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}


//Logging for library code
#ifndef EZ_DONT_INCLUDE_CORE_MACROS
#define EZ_CORE_ABORT() abort()
#define EZ_CORE_EXIT(x) exit(x)

#define EZ_CORE_DEBUG_DEALLOC(...)	ez::Logger::log_core(ez::Logger::Level::DEALLOC, __VA_ARGS__)
#define EZ_CORE_DEBUG_ALLOC(...)	ez::Logger::log_core(ez::Logger::Level::ALLOC, __VA_ARGS__)
#define EZ_CORE_DEBUG(...)			ez::Logger::log_core(ez::Logger::Level::DEBUGLOG, __VA_ARGS__)
#define EZ_CORE_LOG(...)			ez::Logger::log_core(ez::Logger::Level::LOG, __VA_ARGS__)
#define EZ_CORE_WARN(...)			ez::Logger::log_core(ez::Logger::Level::WARNING, __VA_ARGS__)
#define EZ_CORE_ERROR(...)			ez::Logger::log_core(ez::Logger::Level::ERROR, __VA_ARGS__);
#define EZ_CORE_FATAL_ERROR(...) EZ_CORE_ERROR(__VA_ARGS__); EZ_CORE_EXIT(-1)


//Debug Assertions
#ifdef EZ_BUILD_ENABLE_ASSERTIONS
#define EZ_CORE_ASSERT(test, ...) if((test) == false) { ez::Logger::log_core(ez::Logger::Level::DEBUGLOG,  __FILE__, " (", __LINE__, "): Assertion Failed: ", __VA_ARGS__); EZ_CORE_ABORT(); }
#define EZ_CORE_STATIC_ASSERT(test, msg) static_assert(test, msg)
#else
#define EZ_CORE_ASSERT(test, ...)
#define EZ_CORE_STATIC_ASSERT(test, msg) 
#endif
#endif

//Logging for client code
#define EZ_DEBUG(...)	ez::Logger::log_client(ez::Logger::Level::DEBUGLOG, __VA_ARGS__)
#define EZ_LOG(...)		ez::Logger::log_client(ez::Logger::Level::LOG, __VA_ARGS__)
#define EZ_WARN(...)	ez::Logger::log_client(ez::Logger::Level::WARNING, __VA_ARGS__)
#define EZ_ERROR(...)	ez::Logger::log_client(ez::Logger::Level::ERROR, __VA_ARGS__);

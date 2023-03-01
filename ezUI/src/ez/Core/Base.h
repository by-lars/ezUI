#pragma once

#include "ez/Build/BuildConfig.h"
#include <string>
#include <iostream>

namespace ez {
	constexpr int VERSION_MAJOR = 0;
	constexpr int VERSION_MINOR = 1;
	constexpr int VERSION_PATCH = 0;

	enum class Result {
		FAILURE = 0,
		SUCCESS = 1
	};

	class Logger {
	public:
		enum class Level {
			DEALLOC = 0,
			ALLOC,
			DEBUGLOG,
			API,
			LOG,
			WARNING,
			ERROR
		};

		enum class Sender {
			CORE = 0,
			APP,
			API_OGL
		};


		template<class... Args>
		static void Log(Sender sender, Level level, Args... args) {
			if (level < s_CurrentLevel) { return; }

			switch (level) {
			case Level::DEALLOC:	std::cout <<"\x1B[91m[ - ] "; break;
			case Level::ALLOC:		std::cout <<"\x1B[92m[ + ] "; break;
			case Level::DEBUGLOG:	std::cout <<"\x1B[96m[ D ] "; break;
			case Level::LOG:		std::cout <<"\x1B[94m[ I ] "; break;
			case Level::WARNING:	std::cout <<"\x1B[93m[ W ] "; break;
			case Level::ERROR:		std::cout <<"\x1B[41m[ E ] "; break;
			}

			switch (sender) {
			case Sender::CORE:		std::cout << "[ CORE ] "; break;
			case Sender::APP:		std::cout << "[ APP  ] "; break;
			case Sender::API_OGL:	std::cout << "[ OGL  ] "; break;
			}


			(std::cout << ... << args) << "\x1B[0m" << std::endl;
		}

		static void SetLevel(Level level);
		static void Init();
	private:
		static Level s_CurrentLevel;
	};

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}


//Logging for library code
#ifndef EZ_DONT_INCLUDE_CORE_MACROS
#define EZ_CORE_ABORT() abort()
#define EZ_CORE_EXIT(x) exit(x)

#define EZ_CORE_DEBUG_DEALLOC(...)	ez::Logger::Log(ez::Logger::Sender::CORE, ez::Logger::Level::DEALLOC, __VA_ARGS__)
#define EZ_CORE_DEBUG_ALLOC(...)	ez::Logger::Log(ez::Logger::Sender::CORE, ez::Logger::Level::ALLOC, __VA_ARGS__)
#define EZ_CORE_LOG_API(...)		ez::Logger::Log(ez::Logger::Sender::CORE, ez::Logger::Level::API, __VA_ARGS__)
#define EZ_CORE_DEBUG(...)			ez::Logger::Log(ez::Logger::Sender::CORE, ez::Logger::Level::DEBUGLOG, __VA_ARGS__)
#define EZ_CORE_LOG(...)			ez::Logger::Log(ez::Logger::Sender::CORE, ez::Logger::Level::LOG, __VA_ARGS__)
#define EZ_CORE_WARN(...)			ez::Logger::Log(ez::Logger::Sender::CORE, ez::Logger::Level::WARNING, __VA_ARGS__)
#define EZ_CORE_ERROR(...)			ez::Logger::Log(ez::Logger::Sender::CORE, ez::Logger::Level::ERROR, __VA_ARGS__);
#define EZ_CORE_FATAL_ERROR(...) EZ_CORE_ERROR(__VA_ARGS__); EZ_CORE_EXIT(-1)


//Debug Assertions
#ifdef EZ_BUILD_ENABLE_ASSERTIONS
#define EZ_CORE_ASSERT(test, ...) if((test) == false) { ez::Logger::Log(ez::Logger::Sender::CORE, ez::Logger::Level::DEBUGLOG,  __FILE__, " (", __LINE__, "): Assertion Failed: ", __VA_ARGS__); EZ_CORE_ABORT(); }
#define EZ_CORE_STATIC_ASSERT(test, msg) static_assert(test, msg)
#else
#define EZ_CORE_ASSERT(test, ...)
#define EZ_CORE_STATIC_ASSERT(test, msg) 
#endif
#endif

//Logging for client code
#define EZ_DEBUG(...)	ez::Logger::Log(ez::Logger::Sender::APP, ez::Logger::Level::DEBUGLOG, __VA_ARGS__)
#define EZ_LOG(...)		ez::Logger::Log(ez::Logger::Sender::APP, ez::Logger::Level::LOG, __VA_ARGS__)
#define EZ_WARN(...)	ez::Logger::Log(ez::Logger::Sender::APP, ez::Logger::Level::WARNING, __VA_ARGS__)
#define EZ_ERROR(...)	ez::Logger::Log(ez::Logger::Sender::APP, ez::Logger::Level::ERROR, __VA_ARGS__);

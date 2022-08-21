#pragma once

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

	enum class LogLevel {
		DEALLOC = 0,
		ALLOC,
		DEBUGLOG,
		API,
		LOG,
		WARNING,
		ERROR
	};

	enum class LogSender {
		CORE = 0,
		APP,
		API_OGL
	};

	class Logger {
	public:
		template<class... Args>
		static void Log(LogSender sender, LogLevel level, Args... args) {
			if (level < s_CurrentLevel) { return; }

			switch (level) {
			case LogLevel::DEALLOC: std::cout <<"\x1B[91m[ - ] "; break;
			case LogLevel::ALLOC: std::cout <<"\x1B[92m[ + ] "; break;
			case LogLevel::DEBUGLOG: std::cout <<"\x1B[96m[ D ] "; break;
			case LogLevel::LOG: std::cout <<"\x1B[94m[ I ] "; break;
			case LogLevel::WARNING: std::cout <<"\x1B[93m[ W ] "; break;
			case LogLevel::ERROR: std::cout <<"\x1B[41m[ E ] "; break;
			}

			switch (sender) {
			case LogSender::CORE: std::cout << "[ CORE ] "; break;
			case LogSender::APP: std::cout << "[ APP  ] "; break;
			case LogSender::API_OGL: std::cout << "[ OGL  ] "; break;
			}


			(std::cout << ... << args) << "\x1B[0m" << std::endl;
		}

		static void SetLevel(LogLevel level);
		static void Init();
	private:
		static LogLevel s_CurrentLevel;
	};
}

#define EZ_CORE_ABORT() abort()
#define EZ_CORE_EXIT(x) exit(x)

//Logging for library code
#define EZ_CORE_DEBUG_DEALLOC(...) ez::Logger::Log(ez::LogSender::CORE, ez::LogLevel::DEALLOC, __VA_ARGS__)
#define EZ_CORE_DEBUG_ALLOC(...) ez::Logger::Log(ez::LogSender::CORE, ez::LogLevel::ALLOC, __VA_ARGS__)
#define EZ_CORE_LOG_API(...) ez::Logger::Log(ez::LogSender::CORE, ez::LogLevel::API, __VA_ARGS__)
#define EZ_CORE_DEBUG(...) ez::Logger::Log(ez::LogSender::CORE, ez::LogLevel::DEBUGLOG, __VA_ARGS__)
#define EZ_CORE_LOG(...) ez::Logger::Log(ez::LogSender::CORE, ez::LogLevel::LOG, __VA_ARGS__)
#define EZ_CORE_WARN(...) ez::Logger::Log(ez::LogSender::CORE, ez::LogLevel::WARNING, __VA_ARGS__)
#define EZ_CORE_ERROR(...) ez::Logger::Log(ez::LogSender::CORE, ez::LogLevel::ERROR, __VA_ARGS__);
#define EZ_CORE_FATAL_ERROR(...) EZ_CORE_ERROR(__VA_ARGS__); EZ_CORE_EXIT(-1)


//Logging for client code
#define EZ_DEBUG(...) ez::Logger::Log(ez::LogSender::APP, ez::LogLevel::DEBUGLOG, __VA_ARGS__)
#define EZ_LOG(...) ez::Logger::Log(ez::LogSender::APP, ez::LogLevel::LOG, __VA_ARGS__)
#define EZ_WARN(...) ez::Logger::Log(ez::LogSender::APP, ez::LogLevel::WARNING, __VA_ARGS__)
#define EZ_ERROR(...) ez::Logger::Log(ez::LogSender::APP, ez::LogLevel::ERROR, __VA_ARGS__);

//Debug Assertions
#ifdef DEBUG
#define EZ_CORE_ASSERT(test, ...) if((test) == false) { ez::Logger::Log(ez::LogSender::CORE, ez::LogLevel::DEBUGLOG,  __FILE__, " (", __LINE__, "): Assertion Failed: ", __VA_ARGS__); EZ_CORE_ABORT(); }
#define EZ_CORE_STATIC_ASSERT(test, msg) static_assert(test, msg)
#else
#define EZ_CORE_ASSERT(test, ...)
#define EZ_CORE_STATIC_ASSERT(test, msg) 
#endif

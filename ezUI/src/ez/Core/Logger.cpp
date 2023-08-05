#include "ez/Core/Logger.hpp"
#include "ez/Build/BuildConfig.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace ez {
#if EZ_BUILD_DEBUG_MODE
	Logger::Level Logger::s_current_level = Logger::Level::DEALLOC;
#else
	Logger::Level Logger::s_current_level = Logger::Level::LOG;
#endif

	void Logger::set_level(Logger::Level level) {
        s_current_level = level;
	}

	void Logger::init() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
#endif
	}
}
#include "Core/Base.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace ez {
#if EZ_BUILD_DEBUG_MODE
	Logger::Level Logger::s_CurrentLevel = Logger::Level::DEALLOC;
#else
	LogLevel Logger::s_CurrentLevel = LogLevel::LOG;
#endif

	void Logger::SetLevel(Logger::Level level) {
		s_CurrentLevel = level;
	}

	void Logger::Init() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
#endif
	}
}
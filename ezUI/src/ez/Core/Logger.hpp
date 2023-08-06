#pragma once

#include <iostream>
#include <iomanip>

namespace ez {
    class Logger {
    public:
        enum class Level {
            DEALLOC = 0,
            ALLOC,
            DEBUGLOG,
            LOG,
            WARNING,
            ERROR
        };

        /**
         * Prints a formatted log message to the console
         * @tparam Args Type of message parts to be printed (VARARGS)
         * @param sender The sender of the message
         * @param level The importance of the log message
         * @param args Message parts to be printed (VARARGS)
         */
        template<class... Args>
        static void log(const std::string& sender, Level level, Args... args) {
            if (level < s_current_level) { return; }

            switch (level) {
                case Level::DEALLOC:	std::cout <<"\x1B[91m[ - ] "; break;
                case Level::ALLOC:		std::cout <<"\x1B[92m[ + ] "; break;
                case Level::DEBUGLOG:	std::cout <<"\x1B[96m[ D ] "; break;
                case Level::LOG:		std::cout <<"\x1B[94m[ I ] "; break;
                case Level::WARNING:	std::cout <<"\x1B[93m[ W ] "; break;
                case Level::ERROR:		std::cout <<"\x1B[41m[ E ] "; break;
            }

            std::cout << "[ " << sender << " ] ";

            (std::cout << ... << args) << "\x1B[0m" << std::endl;
        }

        /**
         * Prints a formatted log message with the sender being "EZ"
         * Use this for library code only.
         *
         * @tparam Args Type of message parts to be printed (VARARGS)
         * @param level The importance of the log message
         * @param args Message parts to be printed (VARARGS)
         */
        template<class... Args>
        static void log_core(Level level, Args&&... args) {
            log("EZ", level, std::forward<Args>(args)...);
        }

        /**
        * Prints a formatted log message with the sender being "APP"
        * Use this for client code only.
        *
        * @tparam Args Type of message parts to be printed (VARARGS)
        * @param level The importance of the log message
        * @param args Message parts to be printed (VARARGS)
        */
        template<class... Args>
        static void log_client(Level level, Args&&... args) {
            log("APP", level, std::forward<Args>(args)...);
        }

        /**
         * Sets the current log level filter.
         * Setting the level will print only messages up to that level.
         *
         * @param level The new level
         */
        static void set_level(Level level);

        /**
         * Sets up console output
         */
        static void init();

    private:
        static Level s_current_level;
    };
}
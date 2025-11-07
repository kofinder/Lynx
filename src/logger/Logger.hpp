/**
 * @file Logger.hpp
 * @brief Provides a centralized logging system using spdlog for the Lynx project.
 *
 * The `LynxLogger` namespace encapsulates logging functionality with a
 * singleton `LogManager` that manages global and named loggers.
 * It supports colored console output and file logging asynchronously.
 *
 * **Key Features:**
 * - Singleton `LogManager` for consistent logger access.
 * - Global logger and named loggers.
 * - Convenient macros for debug, info, warning, and error messages.
 * - Automatic source file, line number, and function tagging in logs.
 * - Uses spdlog for efficient and thread-safe logging.
 *
 * **Logging Macros:**
 * - `LOG_DEBUG(msg, ...)`
 * - `LOG_INFO(msg, ...)`
 * - `LOG_WARN(msg, ...)`
 * - `LOG_ERROR(msg, ...)`
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_LOGGER_HPP
#define LYNX_LOGGER_HPP

#include "strings.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


namespace LynxLogger {

    class LogManager {  

        public:  
        
            ~LogManager() noexcept;  

            static LogManager& instance() noexcept;  

            std::shared_ptr<spdlog::logger> getGlobalLogger() noexcept;  

            std::shared_ptr<spdlog::logger> getLogger(const std::string& name) noexcept;  

        private: 

            LogManager() noexcept;  

            std::shared_ptr<spdlog::logger> logger;  
    };  

    inline std::shared_ptr<spdlog::logger> Logger() noexcept {  
        return LogManager::instance().getGlobalLogger();  
    }  

    inline std::shared_ptr<spdlog::logger> getLogger(const std::string& name) noexcept {  
        return LogManager::instance().getLogger(name);  
    }  

    #define FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)  
    #define LOG_DEBUG(msg, ...) LynxLogger::Logger()->debug("[{}:{} {}] " msg, FILE_NAME, __LINE__, __FUNCTION__, ##__VA_ARGS__)  
    #define LOG_INFO(msg, ...) LynxLogger::Logger()->info("[{}:{} {}] " msg, FILE_NAME, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define LOG_WARN(msg, ...) LynxLogger::Logger()->warn("[{}:{} {}] " msg, FILE_NAME, __LINE__, __FUNCTION__, ##__VA_ARGS__)  
    #define LOG_ERROR(msg, ...) LynxLogger::Logger()->error("[{}:{} {}] " msg, FILE_NAME, __LINE__, __FUNCTION__, ##__VA_ARGS__)  
}

#endif
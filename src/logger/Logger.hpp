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
        
            // Destructor  
            ~LogManager() noexcept;  

            // Returns the singleton instance of LogManager  
            static LogManager& instance() noexcept;  

            // Returns a logger instance  
            std::shared_ptr<spdlog::logger> getGlobalLogger() noexcept;  

            // Returns a logger instance with a specific name  
            std::shared_ptr<spdlog::logger> getLogger(const std::string& name) noexcept;  

        private:  
            // Private constructor for singleton  
            LogManager() noexcept;  

            // Singleton logger instance  
            std::shared_ptr<spdlog::logger> logger;  
    };  

    // Inline functions  
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
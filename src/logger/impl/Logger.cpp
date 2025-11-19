#include "Logger.hpp"  

namespace LynxLogger {

    static constexpr const char* const k_main_logger = "[LOG]";

    LogManager& LogManager::instance() noexcept {  
        static LogManager instance;
        return instance;  
    }  

    LogManager::LogManager() noexcept {  
        spdlog::init_thread_pool(8192, 1); // Example for async logging  
       logger = spdlog::stdout_color_mt(k_main_logger); // Console logger  
       logger->set_pattern("%^[%Y-%m-%d %H:%M:%S] %n: %v%$"); // Custom log format  
    }  

    LogManager::~LogManager() noexcept {  spdlog::drop_all(); }  

    std::shared_ptr<spdlog::logger> LogManager::getGlobalLogger() noexcept { return logger; }  

    std::shared_ptr<spdlog::logger> LogManager::getLogger(const std::string& name) noexcept {  
        return spdlog::get(name); 
    }  
}

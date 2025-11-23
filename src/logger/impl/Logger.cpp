#include "Logger.hpp"

namespace {
    constexpr std::size_t kAsyncQueueSize = 8192;
    constexpr std::size_t kAsyncThreads = 1;
}

namespace LynxLogger {

    static constexpr const char* const k_main_logger = "[LOG]";

    LogManager& LogManager::instance() noexcept {  
        static LogManager instance;
        return instance;  
    }  

    LogManager::LogManager() noexcept {  
        spdlog::init_thread_pool(kAsyncQueueSize, kAsyncThreads);
        logger = spdlog::stdout_color_mt(k_main_logger); 
        logger->set_pattern("%^[%Y-%m-%d %H:%M:%S] %n: %v%$");
    }  

    LogManager::~LogManager() noexcept {  spdlog::drop_all(); }  

    std::shared_ptr<spdlog::logger> LogManager::getGlobalLogger() noexcept { return logger; }  

    std::shared_ptr<spdlog::logger> LogManager::getLogger(const std::string& name) noexcept {  
        return spdlog::get(name); 
    }  
}

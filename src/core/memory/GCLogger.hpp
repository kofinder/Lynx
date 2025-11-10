#ifndef LYNX_CORE_GC_LOGGER_HPP
#define LYNX_CORE_GC_LOGGER_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include <cmath>

namespace LynxCore {

    class GCLogger {

        public:

            enum class Level { INFO, WARN, ERROR, DEBUG };

            static void log(Level level, const std::string& message) {
                std::lock_guard<std::mutex> lock(getMutex());

                std::ostringstream oss;
                oss << "[" << timestamp() << "] "
                    << "[" << threadId() << "] "
                    << "[" << levelToString(level) << "] "
                    << message;

                std::cout << colorFor(level) << oss.str() << "\033[0m" << std::endl;
            }

            static void info(const std::string& msg) { log(Level::INFO, msg); }
            static void warn(const std::string& msg) { log(Level::WARN, msg); }
            static void error(const std::string& msg) { log(Level::ERROR, msg); }
            static void debug(const std::string& msg) { log(Level::DEBUG, msg); }

            // === GC timing ===
            static std::chrono::steady_clock::time_point startTimer() {
                return std::chrono::steady_clock::now();
            }

            static double stopTimer(std::chrono::steady_clock::time_point start) {
                auto end = std::chrono::steady_clock::now();
                return std::chrono::duration<double, std::milli>(end - start).count();
            }

            // === Visualization ===
            static void visualizeHeap(size_t edenCount, size_t survivorCount, size_t oldCount) {
                size_t total = edenCount + survivorCount + oldCount;
                if (total == 0) total = 1; // avoid div/0

                auto bar = [](size_t count, size_t total, const std::string& label, const char* color) {
                    size_t barWidth = static_cast<size_t>(std::round((count * 30.0) / total));
                    std::ostringstream oss;
                    oss << color << std::setw(8) << std::left << label << ": ";
                    for (size_t i = 0; i < barWidth; ++i) oss << "█";
                    for (size_t i = barWidth; i < 30; ++i) oss << " ";
                    oss << " " << count << "\033[0m";
                    return oss.str();
                };

                std::cout << bar(edenCount, total, "Eden", "\033[1;32m") << "\n"
                        << bar(survivorCount, total, "Survivor", "\033[1;33m") << "\n"
                        << bar(oldCount, total, "OldGen", "\033[1;35m") << "\n"
                        << std::string(48, '-') << std::endl;
            }

        private:

            static std::mutex& getMutex() {
                static std::mutex mtx;
                return mtx;
            }

            static std::string timestamp() {
                using namespace std::chrono;
                auto now = system_clock::now();
                auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
                std::time_t t = system_clock::to_time_t(now);

                std::tm tm{};
                #ifdef _WIN32
                    localtime_s(&tm, &t);
                #else
                    localtime_r(&t, &tm);
                #endif
                std::ostringstream oss;
                oss << std::put_time(&tm, "%H:%M:%S") << '.' << std::setw(3)
                    << std::setfill('0') << ms.count();
                return oss.str();
            }

            static std::string threadId() {
                std::ostringstream oss;
                oss << std::this_thread::get_id();
                return oss.str();
            }

            static const char* levelToString(Level lvl) {
                switch (lvl) {
                    case Level::INFO:  return "INFO";
                    case Level::WARN:  return "WARN";
                    case Level::ERROR: return "ERROR";
                    case Level::DEBUG: return "DEBUG";
                }
                return "UNKNOWN";
            }

            static const char* colorFor(Level lvl) {
                switch (lvl) {
                    case Level::INFO:  return "\033[1;36m"; // cyan
                    case Level::WARN:  return "\033[1;33m"; // yellow
                    case Level::ERROR: return "\033[1;31m"; // red
                    case Level::DEBUG: return "\033[1;90m"; // gray
                }
                return "\033[0m";
            }
    };

}

#endif

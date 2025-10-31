#ifndef LYNX_DATE_DATE_TYPE_HPP  
#define LYNX_DATE_DATE_TYPE_HPP  

#include <string>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace LynxConstants {

    struct Date {
        int year = 0;
        int month = 0;
        int day = 0;
    
        Date() = default;
        Date(int y, int m, int d) : year(y), month(m), day(d) {}
    
        static Date today() {
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::tm localTime{};
            #ifdef _WIN32
                localtime_s(&localTime, &t);
             #else
                localtime_r(&t, &localTime);
            #endif
            return Date(localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday);
        }
    
        std::string toString() const {
            std::ostringstream oss;
            oss << std::setfill('0') << std::setw(4) << year << "-"
                << std::setw(2) << month << "-"
                << std::setw(2) << day;
            return oss.str();
        }
    };    

} 

#endif 


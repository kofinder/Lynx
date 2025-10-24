#ifndef LYNX_DATE_TIME_DATE_TYPE_HPP  
#define LYNX_DATE_TIME_DATE_TYPE_HPP  

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <cmath>

namespace LynxConstants {

    struct DateTime {
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;
        int millisecond = 0;
        int timeZoneOffsetMinutes = 0;

        DateTime() = default;

        DateTime(int y, int m, int d, int h, int min, int s, int ms = 0, int tzOffset = 0)
            : year(y), month(m), day(d),
              hour(h), minute(min), second(s),
              millisecond(ms), timeZoneOffsetMinutes(tzOffset) {}

        static DateTime now() {
            using namespace std::chrono;

            auto now = system_clock::now();
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            std::time_t t = system_clock::to_time_t(now);
            std::tm localTime{};
            
            #ifdef _WIN32
                localtime_s(&localTime, &t);
            #else
                localtime_r(&t, &localTime);
            #endif

            return DateTime(
                localTime.tm_year + 1900,
                localTime.tm_mon + 1,
                localTime.tm_mday,
                localTime.tm_hour,
                localTime.tm_min,
                localTime.tm_sec,
                static_cast<int>(ms.count()),
                0 // Time zone offset; set if needed
            );
        }

        std::string toString(bool useAmPm = false) const {
            std::ostringstream oss;
            int displayHour = hour;
            std::string ampm;

            if (useAmPm) {
                ampm = (hour >= 12) ? " PM" : " AM";
                displayHour = (hour % 12 == 0) ? 12 : hour % 12;
            }

            oss << std::setfill('0') << std::setw(4) << year << "-"
                << std::setw(2) << month << "-"
                << std::setw(2) << day << "T"
                << std::setw(2) << displayHour << ":"
                << std::setw(2) << minute << ":"
                << std::setw(2) << second;

            if (millisecond > 0) {
                oss << "." << std::setw(3) << millisecond;
            }

            if (timeZoneOffsetMinutes != 0) {
                int absOffset = std::abs(timeZoneOffsetMinutes);
                int offsetHours = absOffset / 60;
                int offsetMinutes = absOffset % 60;
                oss << (timeZoneOffsetMinutes >= 0 ? "+" : "-")
                    << std::setw(2) << offsetHours << ":"
                    << std::setw(2) << offsetMinutes;
            } else {
                oss << "Z";
            }

            return oss.str() + ampm;
        }
    };

}

#endif

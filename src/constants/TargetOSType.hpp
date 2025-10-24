#ifndef LYNX_TARGET_OS_TYPE_HPP  
#define LYNX_TARGET_OS_TYPE_HPP  

#include <string>
#include <algorithm>

namespace LynxConstants {

    enum class TargetOSType { windows, linux, custom_linux, freebsd, macos, android, ios, ps4, ps5, web, error };

    inline TargetOSType operator++(TargetOSType &os, int) {
        TargetOSType old = os;
        switch (os) {
            case TargetOSType::windows:      os = TargetOSType::linux; break;
            case TargetOSType::linux:        os = TargetOSType::custom_linux; break;
            case TargetOSType::custom_linux: os = TargetOSType::freebsd; break;
            case TargetOSType::freebsd:      os = TargetOSType::macos; break;
            case TargetOSType::macos:        os = TargetOSType::android; break;
            case TargetOSType::android:      os = TargetOSType::ios; break;
            case TargetOSType::ios:          os = TargetOSType::ps4; break;
            case TargetOSType::ps4:          os = TargetOSType::ps5; break;
            case TargetOSType::ps5:          os = TargetOSType::web; break;
            case TargetOSType::web:          os = TargetOSType::error; break;
            case TargetOSType::error:        os = TargetOSType::windows; break;
        }
        return old;
    }

    inline TargetOSType parseOSTypeToString(const std::string &os) {
        std::string lcase = os;
        std::transform(lcase.begin(), lcase.end(), lcase.begin(), [](unsigned char c){ return std::tolower(c); });

        if (lcase == "windows") return TargetOSType::windows;
        if (lcase == "linux") return TargetOSType::linux;
        if (lcase == "custom_linux") return TargetOSType::custom_linux;
        if (lcase == "freebsd") return TargetOSType::freebsd;
        if (lcase == "macos") return TargetOSType::macos;
        if (lcase == "android") return TargetOSType::android;
        if (lcase == "ios") return TargetOSType::ios;
        if (lcase == "ps4") return TargetOSType::ps4;
        if (lcase == "ps5") return TargetOSType::ps5;
        if (lcase == "web") return TargetOSType::web;
        return TargetOSType::error;
    }

    inline std::string parseStringToOSType(TargetOSType os) {
        switch (os) {
            case TargetOSType::windows:      return "Windows";
            case TargetOSType::linux:        return "Linux";
            case TargetOSType::custom_linux: return "Custom_Linux";
            case TargetOSType::freebsd:      return "FreeBSD";
            case TargetOSType::macos:        return "MacOS";
            case TargetOSType::android:      return "Android";
            case TargetOSType::ios:          return "iOS";
            case TargetOSType::ps4:          return "PS4";
            case TargetOSType::ps5:          return "PS5";
            case TargetOSType::web:          return "Web";
            case TargetOSType::error:        return "Error";
        }
        return "Unknown";
    }

    inline std::string toLowerString(TargetOSType os) {
        std::string str = parseStringToOSType(os);
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
        return str;
    }

    inline TargetOSType GetHostOS() {
        #if defined(_WIN32) || defined(_WIN64)
            return TargetOSType::windows;
        #elif defined(__linux__)
            return TargetOSType::linux;
        #elif defined(__FreeBSD__)
            return TargetOSType::freebsd;
        #elif defined(__APPLE__) && defined(__MACH__)
            return TargetOSType::macos;
        #elif defined(__ANDROID__)
            return TargetOSType::android;
        #elif defined(__unix__)
            return TargetOSType::custom_linux;
        #else
            return TargetOSType::error;
        #endif
    }

}

#endif // LYNX_TARGET_OS_TYPE_HPP




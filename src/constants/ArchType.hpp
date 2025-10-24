#ifndef LYNX_ARCH_TYPE_HPP
#define LYNX_ARCH_TYPE_HPP

#include <string>
#include <algorithm>

namespace LynxConstants {

    enum class ArchType { none, x86, x86_64, arm, aarch64, wasm32, wasm64, xe64, error };

    inline ArchType operator++(ArchType &arch, int) {
        ArchType old = arch;
        switch (arch) {
            case ArchType::none:    arch = ArchType::x86; break;
            case ArchType::x86:     arch = ArchType::x86_64; break;
            case ArchType::x86_64:  arch = ArchType::arm; break;
            case ArchType::arm:     arch = ArchType::aarch64; break;
            case ArchType::aarch64: arch = ArchType::wasm32; break;
            case ArchType::wasm32:  arch = ArchType::wasm64; break;
            case ArchType::wasm64:  arch = ArchType::xe64; break;
            case ArchType::xe64:    arch = ArchType::error; break;
            case ArchType::error:   arch = ArchType::none; break;
        }
        return old;
    }

    inline ArchType parseStringToArchType(const std::string &arch) {
        std::string lcase = arch;
        std::transform(lcase.begin(), lcase.end(), lcase.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (lcase == "none")       return ArchType::none;
        if (lcase == "x86")        return ArchType::x86;
        if (lcase == "x86_64")     return ArchType::x86_64;
        if (lcase == "arm")        return ArchType::arm;
        if (lcase == "aarch64")    return ArchType::aarch64;
        if (lcase == "wasm32")     return ArchType::wasm32;
        if (lcase == "wasm64")     return ArchType::wasm64;
        if (lcase == "xe64")       return ArchType::xe64;

        return ArchType::error;
    }

    inline std::string parseArchTypeToString(ArchType arch) {
        switch (arch) {
            case ArchType::none:    return "none";
            case ArchType::x86:     return "x86";
            case ArchType::x86_64:  return "x86_64";
            case ArchType::arm:     return "arm";
            case ArchType::aarch64: return "aarch64";
            case ArchType::wasm32:  return "wasm32";
            case ArchType::wasm64:  return "wasm64";
            case ArchType::xe64:    return "xe64";
            case ArchType::error:   return "error";
        }
        return "unknown";
    }

}

#endif 

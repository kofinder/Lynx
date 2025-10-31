#ifndef LYNX_RUNTIME_MODULE_TYPE_HPP
#define LYNX_RUNTIME_MODULE_TYPE_HPP

#include <string>
#include <stdexcept>
#include <array>

namespace LynxConstants {

    enum class RuntimeModuleType {
        FILESYSTEM,
        COLLECTIONS,
        DATETIME,
        LOGGER,
        OTHERS,
        __COUNT
    };

    inline RuntimeModuleType operator++(RuntimeModuleType& type, int) {
        RuntimeModuleType old = type;
        if (type == RuntimeModuleType::LOGGER) {
            throw std::out_of_range("Cannot increment beyond LOGGERS");
        }
        type = static_cast<RuntimeModuleType>(static_cast<int>(type) + 1);
        return old;
    }

    inline std::string toString(RuntimeModuleType type) {
        switch (type) {
            case RuntimeModuleType::FILESYSTEM: return "filesystem";
            case RuntimeModuleType::COLLECTIONS: return "collections";
            case RuntimeModuleType::DATETIME: return "datetime";
            case RuntimeModuleType::LOGGER: return "loggers";
            default: return "unknown";
        }
    }

    inline RuntimeModuleType parseModuleType(const std::string& name) {
        if (name == "filesystem") return RuntimeModuleType::FILESYSTEM;
        if (name == "collections") return RuntimeModuleType::COLLECTIONS;
        if (name == "datetime") return RuntimeModuleType::DATETIME;
        if (name == "logger") return RuntimeModuleType::LOGGER;
        return RuntimeModuleType::OTHERS;
    }

}

#endif

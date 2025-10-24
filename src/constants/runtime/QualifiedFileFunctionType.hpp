#ifndef LYNX_QUALIFIED_FILE_FUNCTION_TYPE_HPP
#define LYNX_QUALIFIED_FILE_FUNCTION_TYPE_HPP

#include <string>
#include <unordered_map>
#include <stdexcept>

namespace LynxConstants {

    enum class QualifiedFileFunctionType {
        Open,
        Read,
        Write,
        Close,
        Exists,
        Delete,
        ReadAll,
        GetSize,
        __Count
    };

    // Increment operator for iteration
    inline QualifiedFileFunctionType operator++(QualifiedFileFunctionType &type, int) {
        QualifiedFileFunctionType old = type;
        if (static_cast<int>(type) + 1 < static_cast<int>(QualifiedFileFunctionType::__Count)) {
            type = static_cast<QualifiedFileFunctionType>(static_cast<int>(type) + 1);
        } else {
            type = QualifiedFileFunctionType::Open;
        }
        return old;
    }

    // Parse from "fs::..." function string
    inline QualifiedFileFunctionType parseFromModuleName(const std::string &name) {
        static const std::unordered_map<std::string, QualifiedFileFunctionType> mapping = {
            {"fs::open", QualifiedFileFunctionType::Open},
            {"fs::read", QualifiedFileFunctionType::Read},
            {"fs::write", QualifiedFileFunctionType::Write},
            {"fs::close", QualifiedFileFunctionType::Close},
            {"fs::exists", QualifiedFileFunctionType::Exists},
            {"fs::delete", QualifiedFileFunctionType::Delete},
            {"fs::readall", QualifiedFileFunctionType::ReadAll},
            {"fs::getsize", QualifiedFileFunctionType::GetSize},
        };

        auto it = mapping.find(name);
        if (it != mapping.end()) {
            return it->second;
        }

        throw std::invalid_argument("Unknown function: " + name);
    }

    // Parse from "Class.method" format
    inline QualifiedFileFunctionType parseFromClassQualifiedName(const std::string &qualifiedName) {
        static const std::unordered_map<std::string, QualifiedFileFunctionType> mapping = {
            {"File.open", QualifiedFileFunctionType::Open},
            {"File.read", QualifiedFileFunctionType::Read},
            {"File.write", QualifiedFileFunctionType::Write},
            {"File.close", QualifiedFileFunctionType::Close},
            {"File.exists", QualifiedFileFunctionType::Exists},
            {"File.delete", QualifiedFileFunctionType::Delete},
            {"File.readall", QualifiedFileFunctionType::ReadAll},
            {"File.getsize", QualifiedFileFunctionType::GetSize},
        };

        auto it = mapping.find(qualifiedName);
        if (it != mapping.end()) {
            return it->second;
        }

        throw std::invalid_argument("Unknown qualified method: " + qualifiedName);
    }

    // Convert enum to "fs::..." string
    inline std::string toModuleQualifiedName(QualifiedFileFunctionType type) {
        switch (type) {
            case QualifiedFileFunctionType::Open: return "fs::open";
            case QualifiedFileFunctionType::Read: return "fs::read";
            case QualifiedFileFunctionType::Write: return "fs::write";
            case QualifiedFileFunctionType::Close: return "fs::close";
            case QualifiedFileFunctionType::Exists: return "fs::exists";
            case QualifiedFileFunctionType::Delete: return "fs::delete";
            case QualifiedFileFunctionType::ReadAll: return "fs::readall";
            case QualifiedFileFunctionType::GetSize: return "fs::getsize";
            default: return "unknown";
        }
    }

    // Convert enum to "Class.method" string
    inline std::string toClassQualifiedName(QualifiedFileFunctionType type) {
        switch (type) {
            case QualifiedFileFunctionType::Open: return "File.open";
            case QualifiedFileFunctionType::Read: return "File.read";
            case QualifiedFileFunctionType::Write: return "File.write";
            case QualifiedFileFunctionType::Close: return "File.close";
            case QualifiedFileFunctionType::Exists: return "File.exists";
            case QualifiedFileFunctionType::Delete: return "File.delete";
            case QualifiedFileFunctionType::ReadAll: return "File.readall";
            case QualifiedFileFunctionType::GetSize: return "File.getsize";
            default: return "unknown";
        }
    }
}

#endif

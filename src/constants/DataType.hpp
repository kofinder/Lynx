#ifndef LYNX_DATA_TYPE
#define LYNX_DATA_TYPE

#include <string>
#include <iostream>
#include <unordered_map>
#include <stdexcept>

namespace LynxConstants {

    enum class GenericTypeKind {
        PRIMITIVE      = 1,
        USER_DEFINED   = 2,
        COLLECTION     = 3,
        WRAPPER        = 4,
        INFERRED       = 5,
        LITERAL        = 6,
        SEQUENTIAL     = 7,
        ASSOCIATIVE    = 8
    };

    enum class DataType {
        // === Primitive Types ===
        VOID       = 1,
        BOOLEAN    = 2,
        BYTE       = 3,
        SHORT      = 4,
        INT        = 5,
        LONG       = 6,
        FLOAT      = 7,
        DOUBLE     = 8,
        CHAR       = 9,
        STRING     = 10,
        NULLPTR,

        // === Date/Time Types ===
        DATE       = 11,
        DATETIME   = 12,

        // === Custom / User-Defined ===
        ENUM       = 13,
        CLAZZ      = 14,
        INTERFACE  = 100,
        MIXIN = 1001,

        // === File and Other Types ===
        FILE       = 15,
        OTHER      = 16,
        AUTO       = 17,

        // === Collections (Composite Types) ===
        COLLECTION = 18,
        SEQUENTIAL = 909,
        ASSOCIATIVE = 809,
        ARRAY      = 19,
        VECTOR     = 20,
        LIST       = 21,
        SET        = 22,
        MAP        = 23,
        STACK      = 24,
        QUEUE      = 25,
        TREE       = 26,
        GRAPH      = 27,
        HEAP       = 28,
        DICT       = 29,

        // === Function & Reference Types ===
        FUNCTION   = 30,
        REFERENCE  = 31,
        POINTER    = 32,

        COUNT
    };

    // Postfix increment operator for DataType
    inline DataType operator++(DataType &type, int) {
        DataType old = type;
        if (static_cast<int>(type) + 1 < static_cast<int>(DataType::COUNT)) {
            type = static_cast<DataType>(static_cast<int>(type) + 1);
        } else {
            type = DataType::OTHER;
        }
        return old;
    }

    inline DataType dataTypeFromString(const std::string &typeStr) {
        static const std::unordered_map<std::string, DataType> mapping = {
            { "void",     DataType::VOID },
            { "boolean",  DataType::BOOLEAN },
            { "byte",     DataType::BYTE },
            { "char",     DataType::CHAR },
            { "short",    DataType::SHORT },
            { "int",      DataType::INT },
            { "long",     DataType::LONG },
            { "float",    DataType::FLOAT },
            { "double",   DataType::DOUBLE },
            { "string",   DataType::STRING },
            { "Date",     DataType::DATE },
            { "DateTime", DataType::DATETIME },
            { "enum",     DataType::ENUM },
            { "class",    DataType::CLAZZ },
            { "mixin",    DataType::MIXIN },
            { "interface", DataType::INTERFACE },
            { "File",     DataType::FILE },
            { "auto",     DataType::AUTO },
            { "array",    DataType::ARRAY },
            { "vector",   DataType::VECTOR },
            { "list",     DataType::LIST },
            { "set",      DataType::SET },
            { "map",      DataType::MAP },
            { "stack",    DataType::STACK },
            { "queue",    DataType::QUEUE },
            { "tree",     DataType::TREE },
            { "graph",    DataType::GRAPH },
            { "heap",     DataType::HEAP },
            { "dict",     DataType::DICT },
            { "function", DataType::FUNCTION },
            { "reference", DataType::REFERENCE },
            { "pointer",  DataType::POINTER }
        };

        auto it = mapping.find(typeStr);
        if (it != mapping.end()) {
            return it->second;
        }

        std::cout << "Unknown data type: " << typeStr << std::endl;
        return DataType::OTHER;
    }

    inline std::string dataTypeToString(DataType type) {
        switch (type) {
            case DataType::VOID:       return "void";
            case DataType::BOOLEAN:    return "boolean";
            case DataType::BYTE:       return "byte";
            case DataType::SHORT:      return "short";
            case DataType::INT:        return "int";
            case DataType::LONG:       return "long";
            case DataType::FLOAT:      return "float";
            case DataType::DOUBLE:     return "double";
            case DataType::CHAR:       return "char";
            case DataType::STRING:     return "string";
            case DataType::DATE:       return "Date";
            case DataType::DATETIME:   return "DateTime";
            case DataType::ENUM:       return "enum";
            case DataType::CLAZZ:      return "class";
            case DataType::MIXIN:      return "mixin";
            case DataType::INTERFACE:  return "interface";
            case DataType::FILE:       return "File";
            case DataType::AUTO:       return "auto";
            case DataType::COLLECTION: return "collection";
            case DataType::ARRAY:      return "array";
            case DataType::VECTOR:     return "vector";
            case DataType::LIST:       return "list";
            case DataType::SET:        return "set";
            case DataType::MAP:        return "map";
            case DataType::STACK:      return "stack";
            case DataType::QUEUE:      return "queue";
            case DataType::TREE:       return "tree";
            case DataType::GRAPH:      return "graph";
            case DataType::HEAP:       return "heap";
            case DataType::DICT:       return "dict";
            case DataType::FUNCTION:   return "function";
            case DataType::REFERENCE:  return "reference";
            case DataType::POINTER:    return "pointer";
            case DataType::OTHER:      return "other";
            default:                   return "unknown";
        }
    }

    inline GenericTypeKind takeKind(DataType type) {
        switch (type) {
            case DataType::VOID:
            case DataType::BOOLEAN:
            case DataType::BYTE:
            case DataType::SHORT:
            case DataType::INT:
            case DataType::LONG:
            case DataType::FLOAT:
            case DataType::DOUBLE:
            case DataType::CHAR:
            case DataType::STRING:
            case DataType::DATE:
            case DataType::DATETIME:
                return GenericTypeKind::PRIMITIVE;

            case DataType::ENUM:
            case DataType::CLAZZ:
            case DataType::INTERFACE:
            case DataType::MIXIN:
                return GenericTypeKind::USER_DEFINED;

            case DataType::ARRAY:
            case DataType::VECTOR:
            case DataType::STACK:
            case DataType::QUEUE:
            case DataType::LIST:
            case DataType::SET:
                return GenericTypeKind::SEQUENTIAL;
                
            case DataType::MAP:
            case DataType::TREE:
            case DataType::GRAPH:
            case DataType::HEAP:
            case DataType::DICT:
                return GenericTypeKind::ASSOCIATIVE;

            case DataType::REFERENCE:
            case DataType::POINTER:
                return GenericTypeKind::WRAPPER;

            case DataType::AUTO:
                return GenericTypeKind::INFERRED;

            default:
                return GenericTypeKind::LITERAL;
        }
    }

    inline std::string typeAbbrev(DataType type) {
        switch (type) {
            case DataType::VOID:       return "v";
            case DataType::BOOLEAN:    return "b";
            case DataType::BYTE:       return "y";
            case DataType::SHORT:      return "s";
            case DataType::INT:        return "i";
            case DataType::LONG:       return "l";
            case DataType::FLOAT:      return "f";
            case DataType::DOUBLE:     return "d";
            case DataType::CHAR:       return "c";
            case DataType::STRING:     return "S";
            case DataType::DATE:       return "D";
            case DataType::DATETIME:   return "T";
            case DataType::ENUM:       return "e";
            case DataType::CLAZZ:      return "C";
            case DataType::INTERFACE:  return "I";
            case DataType::FILE:       return "F";
            case DataType::AUTO:       return "a";
            case DataType::COLLECTION: return "l";
            case DataType::ARRAY:      return "A";
            case DataType::VECTOR:     return "v";
            case DataType::LIST:       return "L";
            case DataType::SET:        return "S";
            case DataType::MAP:        return "M";
            case DataType::STACK:      return "k";
            case DataType::QUEUE:      return "q";
            case DataType::TREE:       return "r";
            case DataType::GRAPH:      return "g";
            case DataType::HEAP:       return "h";
            case DataType::DICT:       return "m";
            case DataType::FUNCTION:   return "F";
            case DataType::REFERENCE:  return "R";
            case DataType::POINTER:    return "P";
            case DataType::OTHER:      return "o";
            default:                   return "u"; // unknown
        }
    }    

}

#endif

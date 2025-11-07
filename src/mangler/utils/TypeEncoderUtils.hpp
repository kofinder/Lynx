/**
 * @file TypeEncoderUtils.hpp
 * @brief Utilities for encoding and serializing types for Itanium-style name mangling.
 *
 * This header provides helper functions to convert LLVM types, BaseType objects,
 * and primitive data types into string representations suitable for mangling.
 * It supports both primitive and user-defined types (classes, interfaces, enums)
 * and includes functions to serialize parameters and names according to the Itanium ABI.
 *
 * Main functionalities:
 * - `serializeType(llvm::Type*)` / `serializeType(DataType)`: Converts a type to a string code.
 * - `encodeNameComponent(const std::string&)`: Encodes a name with its length (e.g., "3foo").
 * - `encodeType(const std::string&)`: Placeholder for encoding string-type arguments.
 * - `encodeLLVMType(llvm::Type*)`: Encodes LLVM types including user-defined types.
 * - `encodeBaseType(const BaseType*)`: Encodes BaseType-derived objects.
 * - `encodeParameters(const ParameterTypes&)`: Serializes a vector of parameter types into mangled form.
 *
 * Example Usage:
 * @code
 * llvm::Type* llvmType = ...;
 * std::string encoded = LynxMangler::encodeLLVMType(llvmType);
 * std::vector<BaseType*> params = ...;
 * std::string mangledParams = LynxMangler::encodeParameters(params);
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_TYPE_ENCODER_UTILS_HPP
#define LYNX_TYPE_ENCODER_UTILS_HPP

#include <vector>
#include <llvm/IR/Value.h>
#include <constants/DataType.hpp>
#include <types/tmpl/TypeChecker.hpp>
#include <types/tmpl/TypeCaster.hpp>
#include <mangler/MangleParamter.hpp>
#include <mangler/constants/ItaniumTypeEncoding.hpp>

namespace LynxMangler {

    using namespace LynxTypes;
    using namespace LynxConstants;

   inline std::string serializeType(llvm::Type* type) {
        if (TypeChecker::is<ByteType>(type)) return BYTE;
        if (TypeChecker::is<ShortType>(type)) return SHORT;
        if (TypeChecker::is<IntegerType>(type)) return INT;
        if (TypeChecker::is<LongType>(type)) return LONG;
        if (TypeChecker::is<FloatType>(type)) return FLOAT;
        if (TypeChecker::is<DoubleType>(type)) return DOUBLE;
        if (TypeChecker::is<CharType>(type)) return CHAR;
        if (TypeChecker::is<BooleanType>(type)) return BOOL;
        if (TypeChecker::is<StringType>(type)) return STRING;
        if (TypeChecker::is<DateTimeType>(type)) return DATETIME;
        if (TypeChecker::is<VoidType>(type)) return VOID;
        return UNKNOWN;
    }

    inline std::string serializeType(DataType type) {
        switch (type) {
            case DataType::VOID:       return VOID;
            case DataType::BOOLEAN:    return BOOL;
            case DataType::BYTE:       return BYTE;
            case DataType::SHORT:      return SHORT;
            case DataType::INT:        return INT;
            case DataType::LONG:       return LONG;
            case DataType::FLOAT:      return FLOAT;
            case DataType::DOUBLE:     return DOUBLE;
            case DataType::CHAR:       return CHAR;
            case DataType::STRING:     return STRING;
            case DataType::DATE:       return DATE;
            case DataType::DATETIME:   return DATETIME;
            case DataType::FILE:       return FILE;
            case DataType::AUTO:       return AUTO;
            case DataType::COLLECTION: return COLLECTION;
            case DataType::ARRAY:      return ARRAY;
            case DataType::VECTOR:     return VECTOR;
            case DataType::LIST:       return LIST;
            case DataType::SET:        return SET;
            case DataType::MAP:        return MAP;
            case DataType::STACK:      return STACK;
            case DataType::QUEUE:      return QUEUE;
            case DataType::TREE:       return TREE;
            case DataType::GRAPH:      return GRAPH;
            case DataType::HEAP:       return HEAP;
            case DataType::DICT:       return DICT;
            case DataType::FUNCTION:   return FUNCTION;
            case DataType::REFERENCE:  return REFERENCE;
            case DataType::POINTER:    return POINTER;
            case DataType::OTHER:      return OTHER;
            default:                   return UNKNOWN;
        }
    }

    inline std::string encodeNameComponent(const std::string& name) {
        return std::to_string(name.length()) + name;
    }

    inline std::string encodeType(const std::string& arg) {
        std::cout << "SOMETHING NEED TO DO ABOUT STRING ARGUMENT ========>" << arg << std::endl;
        return "";
    }

    inline std::string encodeLLVMType(llvm::Type* arg) {
        if(auto clazzType = ClassType::fromLLVMType(arg)) { 
            return CLAZZ + encodeNameComponent(clazzType->originalName());
        } else if(auto ifaceType = InterfaceType::fromLLVMType(arg)) {
            return INTERFACE + encodeNameComponent(ifaceType->originalName());
        } else if(auto enumType = EnumType::fromLLVMType(arg)) {
           return ENUM + encodeNameComponent(enumType->originalName());
        } 
        return serializeType(arg);
    }

    inline std::string encodeBaseType(const BaseType* arg) {
        if(auto clazzType = TypeCasting::castType<const ClassType>(arg)) { 
            return CLAZZ + encodeNameComponent(clazzType->originalName());
        } else if(auto ifaceType = TypeCasting::castType<const InterfaceType>(arg)) {
            return INTERFACE + encodeNameComponent(ifaceType->originalName());
        } else if(auto enumType = TypeCasting::castType<const EnumType>(arg)) {
           return ENUM + encodeNameComponent(enumType->originalName());
        } 

        return serializeType(arg->getTypeTag());
    }

    inline std::string encodeParameters(const ParameterTypes& paramTypes) {
        bool isEmpty = std::visit([](auto&& vecRef) -> bool {
            return vecRef.get().empty();
        }, paramTypes);
    
        if (isEmpty) return UNKNOWN;

        return std::visit([](auto&& vecRef) -> std::string {
            const auto& vec = vecRef.get();
            std::string result;
            for (const auto& elem : vec) {
                using T = std::decay_t<decltype(elem)>;
                using U = std::remove_const_t<T>;
                if constexpr (std::is_same_v<U, std::string>) {
                    result += encodeType(elem);
                } else if constexpr (std::is_same_v<U, BaseType*>) {
                    result += encodeBaseType(elem);
                } else if constexpr (std::is_same_v<U, llvm::Type*>) {
                    result += encodeLLVMType(elem);
                }
            }
            return result;
        }, paramTypes);    
    }
}

#endif

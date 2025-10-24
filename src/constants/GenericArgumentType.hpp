#ifndef LYNX_GENERIC_ARGUMENT_TYPE_HPP
#define LYNX_GENERIC_ARGUMENT_TYPE_HPP

#include <iostream>
#include <variant>
#include <string>
#include <memory>

namespace LynxConstants {

    struct VariableType;

    enum class GenericArgKind {
        TYPE,
        LITERAL
    };

    struct GenericArgumentType {

        std::shared_ptr<VariableType> varType;

        GenericArgKind kind;

        std::size_t size;

        GenericArgumentType() = default;

        explicit GenericArgumentType(std::shared_ptr<VariableType> type) : kind(GenericArgKind::TYPE), varType(std::move(type)), size(0) {}

        explicit GenericArgumentType(int arrSize) : kind(GenericArgKind::LITERAL), varType(nullptr), size(arrSize) {}

        static GenericArgumentType fromType(std::shared_ptr<VariableType> type) {
            return GenericArgumentType(std::move(type));
        } 
           
        static GenericArgumentType fromLiteral(std::size_t arrSize) {
            return GenericArgumentType(static_cast<int>(arrSize));
        }
        
    };
    
}

#endif

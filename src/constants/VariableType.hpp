#ifndef LYNX_VARIABLE_TYPE_HPP
#define LYNX_VARIABLE_TYPE_HPP

#include <variant>
#include <string>
#include <memory>
#include "DataType.hpp"
#include <types/interfaces/BaseType.hpp>
#include "GenericArgumentType.hpp"

/**
 * @struct VariableType
 * 
 * @brief Represents a variable type in the programming language.
 * 
 * This struct encapsulates information about a variable, including its
 * type (as defined by the enum DataType), its name, and an optional
 * pointer to a subtype, allowing for nested or more complex variable structures.
 */

using namespace LynxTypes;

namespace LynxConstants {

    struct VariableType {

        DataType type;

        std::string name;

        std::shared_ptr<VariableType> subType;

        std::vector<std::shared_ptr<VariableType>> genericArguments;

        BaseType* internalType = nullptr; // <== ADD THIS
            
        std::size_t arraySize = 0; 
        
        VariableType() = default;
        
        explicit VariableType(
            DataType t, 
            const std::string& n = "", 
            std::shared_ptr<VariableType> sub = nullptr,
            std::vector<std::shared_ptr<VariableType>> args = {},
            std::size_t size = 0
        ) : type(t), name(n), subType(std::move(sub)), genericArguments(std::move(args)), arraySize(size) {}
    
        VariableType(const VariableType& other)
            : type(other.type), name(other.name),
              subType(other.subType ? std::make_shared<VariableType>(*other.subType) : nullptr),
              genericArguments()  {
            for (const auto& arg : other.genericArguments) {
                genericArguments.push_back(arg ? std::make_shared<VariableType>(*arg) : nullptr);
            }        
        }

        VariableType& operator=(const VariableType& other) {
            if (this != &other) {
                type = other.type;
                name = other.name;
                genericArguments.clear();
                subType = other.subType ? std::make_shared<VariableType>(*other.subType) : nullptr;
                for (const auto& arg : other.genericArguments) {
                    genericArguments.push_back(arg ? std::make_shared<VariableType>(*arg) : nullptr);
                }
            }
            return *this;
        }

        bool isValid() const {
            // Type must not be UNKNOWN or OTHER unless you allow them
            if (type == DataType::COLLECTION || type == DataType::ASSOCIATIVE) return false;
        
            // If internalType is set, we consider it valid
            if (internalType != nullptr) return true;
        
            // If generic args exist, all must be valid
            for (const auto& arg : genericArguments) {
                if (!arg || !arg->isValid()) return false;
            }
        
            // If it's an array or complex type, validate the subtype
            if (subType && !subType->isValid()) return false;
        
            return true;
        }        

        std::shared_ptr<VariableType> getFirstGeneric() const {
            if (genericArguments.empty()) {
                throw std::runtime_error("No generic types available");
            }
            return genericArguments[0];
        }
    
        std::shared_ptr<VariableType> getSecondGeneric() const {
            if (genericArguments.size() < 2) {
                throw std::runtime_error("Second generic type not available");
            }
            return genericArguments[1];
        }

        BaseType* getInternalType() const { return internalType; }
    
        VariableType(VariableType&& other) noexcept = default;

        VariableType& operator=(VariableType&& other) noexcept = default;

        std::unique_ptr<VariableType> clone() const {
            auto cloned = std::make_unique<VariableType>(*this);
            return cloned;
        }        

        ~VariableType() = default;
    };

    inline std::shared_ptr<VariableType> createVariableType(DataType type) {
        return std::make_shared<VariableType>(type);
    }
    
    inline std::shared_ptr<VariableType> createVariableType(DataType type, const VariableType& subType) {
        return std::make_shared<VariableType>(type, "", std::make_shared<VariableType>(subType));
    }
    
    inline std::shared_ptr<VariableType> createVariableType(DataType type, const std::string& name) {
        auto dateType = dataTypeFromString(name);
        return std::make_shared<VariableType>(dateType, name);
    }

    inline std::shared_ptr<VariableType> createVariableType(
        DataType type,
        const std::string& name,
        std::shared_ptr<VariableType> subType,
        const std::vector<std::shared_ptr<VariableType>>& args = {}
    ) {
        return std::make_shared<VariableType>(type, name, std::move(subType), args);
    }

    inline std::shared_ptr<VariableType> createGenericVariableType(
        const std::string& genericName,
        const std::shared_ptr<std::vector<std::shared_ptr<GenericArgumentType>>>& arguments
    ) {
        if (!arguments) {
            std::cerr << "[Error] TYPE argument list is null!" << std::endl;
            return createVariableType(DataType::OTHER, genericName);
        }

        if (arguments->empty()) {
            std::cerr << "[Error] TYPE argument is empty!" << std::endl;
            return createVariableType(DataType::OTHER, genericName);
        }

        auto resolvedType = dataTypeFromString(genericName);
        std::vector<std::shared_ptr<VariableType>> args;
        size_t arraySize = 0;

        for (const auto& arg : *arguments) {
            if (!arg) {
                std::cerr << "[Warning] Null GenericArgumentType encountered." << std::endl;
                continue;
            }

            std::shared_ptr<VariableType> argType;
            if (arg->kind == GenericArgKind::TYPE) {
                argType = arg->varType;
            } else if (arg->kind == GenericArgKind::LITERAL) {
                arraySize = arg->size;
            }

            args.push_back(argType);
        }

        auto varType = createVariableType(resolvedType, genericName, nullptr, args);
        if (resolvedType == DataType::ARRAY && arraySize > 0) {
            varType->arraySize = arraySize;
        }
        
        return varType;
    }
}

#endif

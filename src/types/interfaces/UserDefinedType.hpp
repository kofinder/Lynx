/**
 * @file UserDefinedType.hpp
 * @brief Declares the UserDefinedType class, the base abstraction for all user-defined types.
 *
 * The UserDefinedType class extends BaseType to represent complex, developer-defined data structures
 * such as classes, interfaces, enumerations, and other custom constructs in the Lynx type system.
 * It provides the shared interface for all types that are not built-in or collection-based, supporting
 * polymorphism, cloning, and LLVM-backed value creation.
 *
 * **Key Responsibilities:**
 * - Serves as the common base for user-declared entities like classes, interfaces, enums, and functions.
 * - Enforces consistent value creation semantics across all custom-defined types.
 * - Provides type compatibility checks for user-defined categories.
 * - Integrates with the AstContext to enable type validation and LLVM IR generation.
 *
 * **Typical Derived Types:**
 * - ClassType, InterfaceType, EnumType, DateType, FunctionType
 *
 * **Design Notes:**
 * - Unlike UserDefinedType or CollectionType, this represents higher-level, composite user abstractions.
 * - Derived types must implement `clone()` and type-specific LLVM construction logic.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_USER_DEFINED_TYPE_HPP
#define LYNX_USER_DEFINED_TYPE_HPP

#include "BaseType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    using namespace LynxContext;

    class UserDefinedType : public BaseType {

        public:
        
            explicit UserDefinedType(AstContext* context) : BaseType(context) {}
            
            // Rule of five: allow default destructor, delete others
            ~UserDefinedType() override = default;
            UserDefinedType(const UserDefinedType&) = delete;
            UserDefinedType& operator=(const UserDefinedType&) = delete;
            UserDefinedType(UserDefinedType&&) = delete;
            UserDefinedType& operator=(UserDefinedType&&) = delete;

            bool isUserDefinedType() const noexcept override { return true; }

            llvm::Value* createValue(LValueType /*unused*/) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

            llvm::Value* createValue(std::vector<llvm::Value*> /*unused*/) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> /*unused*/) const override {
                astContext->reportError(makeRuntimeError(" createValue ( K, V) doesn't support this createValue signature."));
                return nullptr;  
            }

            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                const auto* obj = dynamic_cast<const UserDefinedType*>(other);
                if (!obj) return false;
                switch (other->getTypeTag()) {
                    case DataType::CLAZZ:
                    case DataType::INTERFACE:
                    case DataType::DATE:
                    case DataType::DATETIME:
                    case DataType::ENUM:
                    case DataType::FILE:
                    case DataType::FUNCTION:   return true;
                    default: return false;
                }
                return false;
            }
    };
}

#endif

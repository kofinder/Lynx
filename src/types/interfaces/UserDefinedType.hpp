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
 * - Unlike BuiltInType or CollectionType, this represents higher-level, composite user abstractions.
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

    class UserDefinedType : public BaseType {

        public:
        
            explicit UserDefinedType(AstContext* context) : BaseType(context) {}

            inline bool isUserDefinedType() const noexcept override { return true; }

            llvm::Value* createValue(LValueType value) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const override {
                astContext->reportError(makeRuntimeError(" createValue ( K, V) doesn't support this createValue signature."));
                return nullptr;  
            }

            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                auto o = dynamic_cast<const UserDefinedType*>(other);
                if (!o) return false;
                switch (other->getTypeTag()) {
                    case DataType::CLAZZ:    return true;
                    case DataType::INTERFACE:   return true;
                    case DataType::DATE:   return true;
                    case DataType::DATETIME:   return true;
                    case DataType::ENUM:   return true;
                    case DataType::FILE:   return true;
                    case DataType::FUNCTION:   return true;
                    default: return false;
                }

                return false;
            }

            virtual std::unique_ptr<BaseType> clone() const override = 0;
    
            virtual ~UserDefinedType() override = default;
    };
}

#endif

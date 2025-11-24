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
 * @date: Jan 2, 2022
*/

#ifndef LYNX_USER_DEFINED_TYPE_HPP
#define LYNX_USER_DEFINED_TYPE_HPP

#include "BaseType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    using namespace LynxContext;

    class UserDefinedType : public BaseType {

        public:
        
            /**
             * @brief Constructs a UserDefinedType with the given AST context.
             * @param context Pointer to the AST context.
            */
            explicit UserDefinedType(AstContext* context) : BaseType(context) {}
            
            // Rule of five: allow default destructor, delete others
            ~UserDefinedType() override = default;
            UserDefinedType(const UserDefinedType&) = delete;
            UserDefinedType& operator=(const UserDefinedType&) = delete;
            UserDefinedType(UserDefinedType&&) = delete;
            UserDefinedType& operator=(UserDefinedType&&) = delete;

            /**
             * @brief Indicates that this is a user-defined type.
             * @return Always true.
            */
            bool isUserDefinedType() const noexcept override { return true; }

            /**
             * @brief Not implemented; returns nullptr.
            */
            llvm::Value* createValue(LValueType /*unused*/) const override { return nullptr; }

            /**
             * @brief Not implemented; returns nullptr.
            */
            llvm::Value* createValue(std::vector<llvm::Value*> /*unused*/) const override { return nullptr; }

            /**
             * @brief Not implemented; returns nullptr.
            */
            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> /*unused*/) const override { return nullptr;  }

            /**
             * @brief Determines if this user-defined type can accept another type.
             *
             * Checks for equality first. Then, if the other type is also a UserDefinedType,
             * verifies that its type tag matches recognized user-defined types.
             *
             * @param other Pointer to another BaseType to check compatibility with.
             * @return True if the type can be accepted, false otherwise.
            */
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
                    case DataType::FUNCTION:   
                        return true;
                    default: 
                        return false;
                }
                return false;
            }
    };
}

#endif

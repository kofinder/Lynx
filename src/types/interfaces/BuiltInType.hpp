/**
 * @file BuiltInType.hpp
 * @brief Declares the BuiltInType class, the abstract base for all built-in primitive types.
 * 
 * The BuiltInType class extends the BaseType interface to represent all primitive data types 
 * directly supported by the Lynx language (e.g., integers, floating-point numbers, characters, strings).
 * It provides default behavior for built-in types, including assignment support, type compatibility checks, 
 * and LLVM size/alignment computation.
 * 
 * **Key Responsibilities:**
 * - Serves as the common superclass for all primitive (non-user-defined) types.
 * - Provides base implementations for assignment and conversion compatibility.
 * - Integrates with LLVM’s DataLayout for size and alignment information.
 * - Handles unsupported value creation signatures gracefully via runtime error reporting.
 * 
 * **Typical Derived Types:**
 * - IntType, FloatType, DoubleType, CharType, StringType, BoolType, etc.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_BUILT_INS_TYPE_HPP
#define LYNX_BUILT_INS_TYPE_HPP

#include "BaseType.hpp"
#include <context/AstContext.hpp>
#include <types/constants/TypeMethodMapConstants.hpp>

namespace LynxTypes {

    class BuiltInType : public BaseType {

        public: 

            /**
             * @brief Constructs a BuiltInType with the given AST context.
             * @param astContext Pointer to the AST context.
            */
            explicit BuiltInType(AstContext* astContext) : BaseType(astContext) {}

            // Rule of five: allow default destructor, delete others
            ~BuiltInType() override = default;
            BuiltInType(const BuiltInType&) = delete;
            BuiltInType& operator=(const BuiltInType&) = delete;
            BuiltInType(BuiltInType&&) = delete;
            BuiltInType& operator=(BuiltInType&&) = delete;

            /**
             * @brief Indicates that this is a built-in type.
             * @return Always true for built-in types.
            */
            bool isBuiltInType() const  noexcept override { return true; }

            /**
             * @brief Indicates that assignment is supported for built-in types.
             * @return Always true.
            */
            bool supportsAssignment() const noexcept override { return true; }

            /**
             * @brief Returns the method resolver for this type, creating it if necessary.
             *
             * This is used to lazily initialize and retrieve the TypeMethodResolver
             * associated with this type for method dispatch or codegen purposes.
             *
             * @return Pointer to a TypeMethodResolver instance.
            */
            virtual TypeMethodResolver* getOrCreateResolver() const = 0;

            /**
             * @brief Not implemented for this overload; returns nullptr.
            */
            llvm::Value* createValue(std::vector<llvm::Value*> /*unused*/) const override { return nullptr; }

            /**
             * @brief Not implemented for this overload; returns nullptr.
            */
            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> /*unused*/) const override { return nullptr; }

            /**
             * @brief Determines if this built-in type can accept another type.
             *
             * Checks for equality first. Then, if the other type is also a BuiltInType,
             * verifies that its type tag matches one of the recognized built-in types.
             *
             * @param other Pointer to another BaseType to check compatibility with.
             * @return True if the type can be accepted, false otherwise.
            */
            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                const auto* obj = dynamic_cast<const BuiltInType*>(other);
                if (!obj) return false;
                switch (other->getTypeTag()) {
                    case DataType::BYTE:
                    case DataType::SHORT:
                    case DataType::INT:
                    case DataType::LONG:
                    case DataType::FLOAT:
                    case DataType::DOUBLE:
                    case DataType::CHAR:
                    case DataType::STRING:  
                        return true;
                    default: 
                        return false;
                }
                return false;
            }
    };
    
}

#endif 

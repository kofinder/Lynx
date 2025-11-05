/**
 * @file FunctionAttributeBuilder.hpp
 * @brief Utility class for constructing and applying LLVM function attributes.
 * 
 * The `FunctionAttributeBuilder` class provides a fluent interface for configuring
 * LLVM function attributes at various levels (function, return value, and parameters).
 * It is used to apply optimization hints and constraints that guide LLVM’s code generation
 * and optimization passes.
 * 
 * **Responsibilities:**
 * - Collect and manage function attributes before applying them.
 * - Support function-level, return-level, and parameter-level attribute application.
 * - Provide a chainable builder API for readability and convenience.
 * 
 * @see llvm::Function, llvm::AttrBuilder, llvm::AttributeList
 * @namespace LynxFunctionAttr
 * Provides helper classes and utilities for managing LLVM function attributes in Lynx.
 * 
 * @note This class simplifies LLVM attribute management, ensuring consistency
 *       across code generation stages within the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNCTION_ATTRS_BUILDER_HPP
#define LYNX_FUNCTION_ATTRS_BUILDER_HPP

#include <llvm/IR/Function.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/LLVMContext.h>


namespace LynxFunctionAttr {

    class FunctionAttributeBuilder {

        private:

            /// Target LLVM function to which attributes will be applied
            llvm::Function* function;

            /// Attributes applied to the function itself (e.g., NoUnwind, AlwaysInline)
            llvm::AttrBuilder fnAttrs;

            /// Attributes applied to the function's return value (e.g., NonNull, NoAlias)
            llvm::AttrBuilder retAttrs;

            /// Attributes applied to each parameter of the function
            std::vector<llvm::AttrBuilder> paramAttrs;


        public:
            /**
             * @brief Construct the builder for a specific function.
             * @param func Pointer to the LLVM function.
             */
            explicit FunctionAttributeBuilder(llvm::Function* func);

            /**
             * @brief Add an attribute to the function.
             * @param attr The attribute kind to add (e.g., NoInline, ReadNone).
             * @return Reference to this builder (for chaining).
             */
            FunctionAttributeBuilder& addAttribute(llvm::Attribute::AttrKind attr);

            /**
             * @brief Add an attribute to the return value of the function.
             * @param attr The attribute kind to add (e.g., NoAlias, NonNull).
             * @return Reference to this builder (for chaining).
             */
            FunctionAttributeBuilder& addAttributeAtRet(llvm::Attribute::AttrKind attr);

            /**
             * @brief Add an attribute to a specific parameter of the function.
             * @param attr The attribute kind to add (e.g., NonNull).
             * @param paramIndex Index of the parameter (starting from 0).
             * @return Reference to this builder (for chaining).
             */
            FunctionAttributeBuilder& addAttributeAtParam(llvm::Attribute::AttrKind attr, unsigned paramIndex);

            /**
             * @brief Apply all accumulated attributes to the function.
             *
             * Combines function-level, return-level, and parameter-level attributes
             * into an AttributeList and applies them to the LLVM function.
             */
            void apply();

    };
}

#endif 

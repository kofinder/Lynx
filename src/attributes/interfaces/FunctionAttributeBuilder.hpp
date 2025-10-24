#ifndef LYNX_FUNCTION_ATTRS_BUILDER_HPP
#define LYNX_FUNCTION_ATTRS_BUILDER_HPP

#include <llvm/IR/Function.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/LLVMContext.h>


namespace LynxFunctionAttr {

    /**
     * @brief Helper class to build and apply LLVM function attributes.
     *
     * FunctionAttributeBuilder provides a convenient way to add attributes at the
     * function level, return value, and individual parameters, and then apply
     * them all at once to the target llvm::Function.
     */

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

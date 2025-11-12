/**
 * @file AlwaysInlineHandler.hpp
 * @brief Handler for applying the LLVM `AlwaysInline` attribute to small functions.
 *
 * @responsibilities
 * - Checks the number of basic blocks in a function.
 * - If the function has 5 or fewer blocks and is defined (not a declaration), applies the `AlwaysInline` attribute.
 * - Part of a chain of function attribute handlers used by the Lynx compiler to optimize function calls.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_ALWAYS_INLINE_HANDLER_HPP
#define LYNX_FUNC_ALWAYS_INLINE_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class AlwaysInlineHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func || func->isDeclaration()) return;
                if (func->size() <= 5) {
                    llvm::LLVMContext &ctx = func->getContext();
                    builder.addAttribute(llvm::Attribute::get(ctx, llvm::Attribute::AlwaysInline));
                    LOG_ERROR("Applied alwaysinline attribute");
                }
            }
    };    

}

#endif

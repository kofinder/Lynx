/**
 * @file NoInlineAsmHandler.hpp
 * @brief Handler for functions marked with the "no-inline-asm" attribute.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects LLVM functions with the "no-inline-asm" attribute.
 * - Optionally applies the corresponding LLVM function attribute to prevent inline assembly.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_FUNC_NO_INLINE_ASM_HANDLER_HPP
#define LYNX_FUNC_NO_INLINE_ASM_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class NoInlineAsmHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;
                if (func->hasFnAttribute("no-inline-asm")) {
                    builder.addAttribute(llvm::Attribute::get(func->getContext(), "no-inline-asm"));
                    LOG_INFO("Applied 'no-inline-asm' attribute");
                }        
            }
    };          

}

#endif

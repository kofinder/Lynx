/**
 * @file StackProtectorHandler.hpp
 * @brief Handler that applies the StackProtect attribute to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects if a function has the "stack-protector" attribute.
 * - Applies the LLVM StackProtect attribute to enable stack smashing protection.
 * - Ensures additional runtime safety for functions susceptible to buffer overflows.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_FUNC_STACK_PROTECTOR_HANDLER_HPP
#define LYNX_FUNC_STACK_PROTECTOR_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class StackProtectorHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;
                if (func->hasFnAttribute("stack-protector")) {
                    llvm::LLVMContext &ctx = func->getContext();
                    builder.addAttribute(llvm::Attribute::get(ctx, llvm::Attribute::StackProtect));
                    LOG_INFO("Applied 'StackProtect' attribute to function {}", func->getName().str());
                }
        
            }
    };        

}

#endif

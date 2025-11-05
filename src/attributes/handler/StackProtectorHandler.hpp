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
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_STACK_PROTECTOR_HANDLER_HPP
#define LYNX_FUNC_STACK_PROTECTOR_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class StackProtectorHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked StackProtectorHandler");
                if (func->hasFnAttribute("stack-protector")) {
                    builder.addAttribute(llvm::Attribute::StackProtect);
                    LOG_INFO("Applied StackProtect attributes");
                }
            }
    };        

}

#endif

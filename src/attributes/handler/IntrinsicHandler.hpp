/**
 * @file IntrinsicHandler.hpp
 * @brief Handler for applying safe LLVM attributes to intrinsic functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects LLVM intrinsic functions and applies default safe attributes.
 * - Sets NoUnwind, WillReturn, and appropriate memory access attributes.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_INTRISIC_HANDLER_HPP
#define LYNX_FUNC_INTRISIC_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class IntrinsicHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked IntrinsicHandler");
                if (func->isIntrinsic()) {
                    // Safe defaults for intrinsic functions
                    builder.addAttribute(llvm::Attribute::NoUnwind);
                    builder.addAttribute(llvm::Attribute::WillReturn);
    
                    // Conservative memory access assumptions
                    if (func->doesNotAccessMemory()) {
                        builder.addAttribute(llvm::Attribute::ReadNone);
                    } else if (func->onlyReadsMemory()) {
                        builder.addAttribute(llvm::Attribute::ReadOnly);
                    }
    
                    LOG_WARN("Applied intrinsic-safe attributes");
                }    
            }
    };
}

#endif

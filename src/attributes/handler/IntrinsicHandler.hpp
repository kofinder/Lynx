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
 * @date:: November 4, 2025
*/

#ifndef LYNX_FUNC_INTRISIC_HANDLER_HPP
#define LYNX_FUNC_INTRISIC_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class IntrinsicHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                if (func->isIntrinsic()) {
                    auto &ctx = func->getContext();
    
                    // Safe defaults for intrinsic functions
                    builder.addAttribute(llvm::Attribute::get(ctx, llvm::Attribute::NoUnwind));
                    builder.addAttribute(llvm::Attribute::get(ctx, llvm::Attribute::WillReturn));
        
                    // Conservative memory access assumptions
                    if (func->doesNotAccessMemory()) {
                        builder.addAttribute(llvm::Attribute::get(ctx, llvm::Attribute::ReadNone));
                    } else if (func->onlyReadsMemory()) {
                        builder.addAttribute(llvm::Attribute::get(ctx, llvm::Attribute::ReadOnly));
                    }
        
                    LOG_INFO("Applied intrinsic-safe attributes");
                }  
            }
    };
}

#endif

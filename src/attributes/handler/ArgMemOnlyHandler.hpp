/**
 * @file ArgMemOnlyHandler.hpp
 * @brief Handler for applying the LLVM `ArgMemOnly` attribute to pointer-type function arguments.
 *
 * @responsibilities
 * - Iterates over all arguments of a given LLVM function.
 * - Detects pointer-type arguments.
 * - Applies the `ArgMemOnly` attribute to each pointer argument using the attribute builder.
 * - Part of the chain of function attribute handlers for optimizing function calls in Lynx compiler.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_ARG_MEM_HANDLER_HPP
#define LYNX_FUNC_ARG_MEM_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class ArgMemOnlyHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked ArgMemOnlyHandler");
                for (auto& arg : func->args()) {
                    if (arg.getType()->isPointerTy()) {
                        builder.addAttributeAtParam(llvm::Attribute::ArgMemOnly, arg.getArgNo());
                        LOG_WARN("Applied arg-memry-only attributes");
                    }
                }
            }
        };        
}

#endif

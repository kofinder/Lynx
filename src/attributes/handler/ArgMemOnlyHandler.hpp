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

#include "FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class ArgMemOnlyHandler : public FunctionAttributeHandler {
        
        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                for (auto& arg : func->args()) {
                    if (arg.getType()->isPointerTy()) {
                        builder.addStringAttributeAtParam("readonly", arg.getArgNo());
                        builder.addStringAttributeAtParam("nocapture", arg.getArgNo());
                        LOG_WARN("Applied ReadOnly and NoCapture to pointer argument #" + std::to_string(arg.getArgNo()));
                    }
                }
            }
    };        
}

#endif

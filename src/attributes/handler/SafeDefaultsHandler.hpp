/**
 * @file SafeDefaultsHandler.hpp
 * @brief Handler that applies safe default attributes to LLVM functions and their arguments.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Marks pointer return types as NonNull.
 * - Marks pointer arguments as NoCapture and ReadOnly.
 * - Provides safe default LLVM function attributes to assist optimizations and safety.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/

#ifndef LYNX_FUNC_SAFE_DEFAULTS_HANDLER_HPP
#define LYNX_FUNC_SAFE_DEFAULTS_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"

#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class SafeDefaultsHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_ERROR("Invoke SafeDefaultsHandler");

                // Safe default for return (if pointer type)
                if (func->getReturnType()->isPointerTy()) {
                    builder.addAttributeAtRet(llvm::Attribute::NonNull);
                }

                unsigned idx = 0;
                for (llvm::Argument &arg : func->args()) {
                    if (arg.getType()->isPointerTy()) {
                        builder.addAttributeAtParam(llvm::Attribute::NoCapture, idx);
                        builder.addAttributeAtParam(llvm::Attribute::ReadOnly, idx);
                    }
                    ++idx;
                }


                LOG_ERROR("Applied Default attributes");
            }
    };    

}

#endif

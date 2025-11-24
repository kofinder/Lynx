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
 * @date:: November 5, 2025
*/

#ifndef LYNX_FUNC_SAFE_DEFAULTS_HANDLER_HPP
#define LYNX_FUNC_SAFE_DEFAULTS_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class SafeDefaultsHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                auto& ctx = func->getContext();

                if (func->getReturnType()->isPointerTy()) {
                    builder.addAttributeAtRet(llvm::Attribute::get(ctx, llvm::Attribute::NonNull));
                    LOG_INFO("Applied 'nonnull' attribute to return of function {}", func->getName().str());
                }
        
                unsigned idx = 0;
                for (llvm::Argument &arg : func->args()) {
                    if (arg.getType()->isPointerTy()) {
                        builder.addAttributeAtParam(llvm::Attribute::get(ctx, "nocapture"), idx);
                        builder.addAttributeAtParam(llvm::Attribute::get(ctx, llvm::Attribute::ReadOnly), idx);
                        LOG_INFO("Applied 'nocapture' and 'readonly' attributes to parameter {} {}",
                                 std::to_string(idx), func->getName().str());
                    }
                    ++idx;
                }
            }
    };    

}

#endif

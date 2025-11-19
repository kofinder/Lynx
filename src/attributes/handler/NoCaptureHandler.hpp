/**
 * @file NoCaptureHandler.hpp
 * @brief Handler for applying NoCapture LLVM attribute to function pointer arguments.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects pointer arguments in LLVM functions.
 * - Applies the NoCapture attribute to pointer parameters to indicate they are not captured.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_NO_CAPTURE_HANDLER_HPP
#define LYNX_FUNC_NO_CAPTURE_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class NoCaptureHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;
        
                for (auto &arg : func->args()) {
                    if (arg.getType()->isPointerTy()) {
                        auto &ctx = func->getContext();
                        builder.addAttributeAtParam(llvm::Attribute::get(ctx, "nocapture"), arg.getArgNo());
                        LOG_INFO("Applied 'nocapture' attribute to argument #{}", std::to_string(arg.getArgNo()));
                    }
                }
            }
    };    

}

#endif

#ifndef LYNX_FUNC_SAFE_DEFAULTS_HANDLER_HPP
#define LYNX_FUNC_SAFE_DEFAULTS_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

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

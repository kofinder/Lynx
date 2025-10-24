#ifndef LYNX_FUNC_NO_CAPTURE_HANDLER_HPP
#define LYNX_FUNC_NO_CAPTURE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class NoCaptureHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoCaptureHandler");
                for (auto& arg : func->args()) {
                    if (arg.getType()->isPointerTy()) {
                        builder.addAttributeAtParam(llvm::Attribute::NoCapture, arg.getArgNo());
                        LOG_WARN("Applied no capture attributes");
                    }
                }
            }
    };    

}

#endif

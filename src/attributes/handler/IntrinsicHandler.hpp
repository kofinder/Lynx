#ifndef LYNX_FUNC_INTRISIC_HANDLER_HPP
#define LYNX_FUNC_INTRISIC_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

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

#ifndef LYNX_FUNC_ARG_MEM_HANDLER_HPP
#define LYNX_FUNC_ARG_MEM_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

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

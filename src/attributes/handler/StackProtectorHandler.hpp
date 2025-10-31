#ifndef LYNX_FUNC_STACK_PROTECTOR_HANDLER_HPP
#define LYNX_FUNC_STACK_PROTECTOR_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class StackProtectorHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked StackProtectorHandler");
                if (func->hasFnAttribute("stack-protector")) {
                    builder.addAttribute(llvm::Attribute::StackProtect);
                    LOG_INFO("Applied StackProtect attributes");
                }
            }
    };        

}

#endif

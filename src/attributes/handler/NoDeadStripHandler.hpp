#ifndef LYNX_FUNC_NO_DEAD_STRIP_HANDLER_HPP
#define LYNX_FUNC_NO_DEAD_STRIP_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class NoDeadStripHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoke NoDeadStripHandler");
                if (func->hasFnAttribute("no-dead-strip")) {
                   // builder.addAttribute(llvm::Attribute::NoDeadStrip);
                }
            }
    };
    
}

#endif

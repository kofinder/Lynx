#ifndef LYNX_FUNC_NO_INLINE_HIT_HANDLER_HPP
#define LYNX_FUNC_NO_INLINE_HIT_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class NoInlineHintHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoInlineHintHandler");
                if (func->hasFnAttribute("no-inline-hint")) {
                    builder.addAttribute(llvm::Attribute::NoInline);
                    LOG_WARN("Applied no-inline-hit attribute");
                }
            }
    };

}

#endif

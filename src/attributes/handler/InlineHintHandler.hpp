#ifndef LYNX_FUNC_INLINE_HIT_HANDLER_HPP
#define LYNX_FUNC_INLINE_HIT_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class InlineHintHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked InlineHintHandler");
                if (func->getName().startswith("hint_inline")) {
                    builder.addAttribute(llvm::Attribute::InlineHint);
                    LOG_WARN("Applied hint_inline attributes");
                }
            }
        };
}        

#endif 

#ifndef LYNX_FUNC_NO_INLINE_HANDLER_HPP
#define LYNX_FUNC_NO_INLINE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class NoInlineHandler : public FunctionAttributeHandler {

        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoInlineHandler");
                if (func->size() > 100) {
                    builder.addAttribute(llvm::Attribute::NoInline);
                    LOG_WARN("Applied no-inline attributes");
                }
            }
    };    

}

#endif

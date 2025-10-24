#ifndef LYNX_FUNC_ALWAYS_INLINE_HANDLER_HPP
#define LYNX_FUNC_ALWAYS_INLINE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class AlwaysInlineHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked AlwaysInlineHandler");

                if (func->size() <= 5 && !func->isDeclaration()) {
                    builder.addAttribute(llvm::Attribute::AlwaysInline);
                    LOG_ERROR("Applied alwayinline attributes");
                }
            }
    };    

}

#endif

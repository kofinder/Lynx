#ifndef LYNX_FUNC_RETURN_TWICE_HANDLER_HPP
#define LYNX_FUNC_RETURN_TWICE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class ReturnsTwiceHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked ReturnsTwiceHandler");
                if (func->getName().contains("setjmp")) {
                    LOG_WARN("Applied setjmp attributes");
                    builder.addAttribute(llvm::Attribute::ReturnsTwice);
                }
            }
        };
              
                
}

#endif

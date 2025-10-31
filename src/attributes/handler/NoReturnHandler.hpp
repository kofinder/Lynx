#ifndef LYNX_FUNC_NO_RETURN_HANDLER_HPP
#define LYNX_FUNC_NO_RETURN_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class NoReturnHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoReturnHandler");
                if (func->getName().startswith("abort") || func->getName().startswith("exit")) {
                    builder.addAttribute(llvm::Attribute::NoReturn);

                    LOG_WARN("Applied abort, exit attribute");
                }
            }
    };    

}

#endif

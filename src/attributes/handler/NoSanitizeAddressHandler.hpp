#ifndef LYNX_FUNC_NO_SANITIZE_ADDRESS_HANDLER_HPP
#define LYNX_FUNC_NO_SANITIZE_ADDRESS_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class NoSanitizeAddressHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoSanitizeAddressHandler");
            }
        };                
}

#endif

#ifndef LYNX_FUNC_SANITIZE_ADDRESS_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_ADDRESS_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class SanitizeAddressHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked SanitizeAddressHandler");
                if (false) {
                    builder.addAttribute(llvm::Attribute::SanitizeAddress);
                    LOG_INFO("Applied SanitizeAddres attribues");
                }
            }
    };
        

}

#endif

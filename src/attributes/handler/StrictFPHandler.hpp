#ifndef LYNX_FUNC_STRICT_FP_HANDLER_HPP
#define LYNX_FUNC_STRICT_FP_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class StrictFPHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked StrictFPHandler");
                if (func->hasFnAttribute("strictfp")) {
                    builder.addAttribute(llvm::Attribute::StrictFP);
                    LOG_INFO("Applied strict-fp attributes");
                }
            }
        };     
                
}

#endif

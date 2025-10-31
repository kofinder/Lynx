#ifndef LYNX_FUNC_SANITIZE_COVERAGE_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_COVERAGE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class SanitizeCoverageHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked SanitizeCoverageHandler");
            }
        };
        
         
                
}

#endif

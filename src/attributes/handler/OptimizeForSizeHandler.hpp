#ifndef LYNX_FUNC_OPTIMIZE_FOR_SIZE_HANDLER_HPP
#define LYNX_FUNC_OPTIMIZE_FOR_SIZE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

class OptimizeForSizeHandler : public FunctionAttributeHandler {
    protected:
        void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
            // LOG_INFO("Invoked OptimizeForSizeHandler");
            if (func->hasFnAttribute("optimize-for-size")) {
                builder.addAttribute(llvm::Attribute::OptimizeForSize);
                LOG_WARN("Applied optimize-for-size attributes");
            }
        }
    };
}

#endif 

#ifndef LYNX_FUNC_READ_ONLY_HANDLER_HPP
#define LYNX_FUNC_READ_ONLY_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class ReadOnlyHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked ReadOnlyHandler");
                if (/* check if function only reads memory */ false) {
                    builder.addAttribute(llvm::Attribute::ReadOnly);
                    LOG_WARN("Applied readonly attributes");
                }
            }
    
    };    

}

#endif

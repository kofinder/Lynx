#ifndef LYNX_FUNC_READ_NONE_HANDLER_HPP
#define LYNX_FUNC_READ_NONE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class ReadNoneHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked ReadNoneHandler");
                if (/* check if function is pure and side-effect free */ false) {
                    builder.addAttribute(llvm::Attribute::ReadNone);
                    LOG_WARN("Applied read-none attributes");
                }
            }    
    
    };    

}

#endif

#ifndef LYNX_FUNC_DEREFRENCEABLE_PARAM_HANDLER_HPP
#define LYNX_FUNC_DEREFRENCEABLE_PARAM_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class DereferenceableParamHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked DereferenceableParamHandler");

                if (func->size() <= 5 && !func->isDeclaration()) {
                    builder.addAttributeAtParam(llvm::Attribute::Dereferenceable, 0);
                    LOG_ERROR("Applied alwayinline attributes");
                }
            }
    };    

}

#endif

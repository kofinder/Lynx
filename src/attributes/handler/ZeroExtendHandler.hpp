#ifndef LYNX_FUNC_ZERO_EXTEND_HANDLER_HPP
#define LYNX_FUNC_ZERO_EXTEND_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class ZeroExtendHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked ZeroExtendHandler");
                llvm::Type* retType = func->getReturnType();

                if (retType->isIntegerTy() && retType->getIntegerBitWidth() > 1) {
                    LOG_ERROR("Applied zero-extend attributes");
                    builder.addAttributeAtRet(llvm::Attribute::ZExt);
                }
            }
    };
                
}

#endif

/**
 * @file ZeroExtendHandler.hpp
 * @brief Handler that applies ZeroExtend (ZExt) attribute to integer return types.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Inspects the return type of LLVM functions.
 * - If the return type is an integer with bit width > 1, applies the ZExt attribute.
 * - Helps LLVM perform type-safe integer widening optimizations.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_ZERO_EXTEND_HANDLER_HPP
#define LYNX_FUNC_ZERO_EXTEND_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

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

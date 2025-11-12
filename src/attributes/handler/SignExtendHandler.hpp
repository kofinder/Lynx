/**
 * @file SignExtendHandler.hpp
 * @brief Handler that applies the SExt (sign-extend) attribute to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Checks the function’s return type.
 * - Applies the SExt attribute for integer return types larger than 1 bit.
 * - Ensures correct sign extension semantics during code generation.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_FUNC_SIGN_EXTEND_HANDLER_HPP
#define LYNX_FUNC_SIGN_EXTEND_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"


namespace LynxFunctionAttr {

    class SignExtendHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                llvm::Type* retType = func->getReturnType();
                if (retType->isIntegerTy() && retType->getIntegerBitWidth() > 1) {
                    builder.addAttributeAtRet(llvm::Attribute::SExt);
                    LOG_ERROR("Applied SExt attributes");
                }
            }
    };
                
}

#endif

/**
 * @file DereferenceableParamHandler.hpp
 * @brief Handler for applying LLVM "dereferenceable" attributes to function parameters.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Determines if function parameters can be marked as dereferenceable for LLVM optimizations.
 * - Improves code generation and memory safety by hinting that certain pointers are always valid.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_FUNC_DEREFRENCEABLE_PARAM_HANDLER_HPP
#define LYNX_FUNC_DEREFRENCEABLE_PARAM_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class DereferenceableParamHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func || func->isDeclaration()) return;
                if (func->size() <= 5) {
                    auto args = func->args().begin();
                    if (args != func->args().end() && args->getType()->isPointerTy()) {
                        llvm::LLVMContext &ctx = func->getContext();
                        builder.addAttributeAtParam(llvm::Attribute::get(ctx, llvm::Attribute::Dereferenceable), 0);
                        LOG_INFO("Applied 'Dereferenceable' attribute to first parameter of function {}", func->getName().str());
                    }
                }
            }
    };    

}

#endif

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
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_DEREFRENCEABLE_PARAM_HANDLER_HPP
#define LYNX_FUNC_DEREFRENCEABLE_PARAM_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class DereferenceableParamHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked DereferenceableParamHandler");

                if (func->size() <= 5 && !func->isDeclaration()) {
                   // builder.addAttributeAtParam(llvm::Attribute::Dereferenceable, 0);
                    LOG_ERROR("Applied alwayinline attributes");
                }
            }
    };    

}

#endif

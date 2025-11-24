/**
 * @file ReturnsTwiceHandler.hpp
 * @brief Handler that applies the LLVM ReturnsTwice attribute to specific functions.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Detects functions (e.g., `setjmp`) that may return multiple times.
 * - Marks such functions with the LLVM `ReturnsTwice` attribute to assist the optimizer.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 5, 2025
*/

#ifndef LYNX_FUNC_RETURN_TWICE_HANDLER_HPP
#define LYNX_FUNC_RETURN_TWICE_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class ReturnsTwiceHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;
                if (func->getName().contains("setjmp")) {
                    builder.addAttribute(llvm::Attribute::get(func->getContext(), llvm::Attribute::ReturnsTwice));
                    LOG_WARN("Applied 'returns_twice' attribute to function {}", func->getName().str());
                }
            }
    };
                      
}

#endif

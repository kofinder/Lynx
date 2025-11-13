/**
 * @file OptimizeForSizeHandler.hpp
 * @brief Handler that applies the LLVM OptimizeForSize attribute to functions.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Detects functions explicitly marked for size optimization.
 * - Applies the LLVM `OptimizeForSize` attribute to guide the backend optimizer.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/

#ifndef LYNX_FUNC_OPTIMIZE_FOR_SIZE_HANDLER_HPP
#define LYNX_FUNC_OPTIMIZE_FOR_SIZE_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class OptimizeForSizeHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                if (func->hasFnAttribute("optimize-for-size")) {
                    builder.addAttribute(llvm::Attribute::get(func->getContext(), "optsize"));
                    LOG_INFO("Applied 'optimize-for-size' attribute to function {}", func->getName().str());
                }        
            }
    };
}

#endif 

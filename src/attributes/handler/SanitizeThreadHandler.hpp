/**
 * @file SanitizeThreadHandler.hpp
 * @brief Handler that applies thread sanitizer attributes to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Intended to mark functions for thread sanitization checks.
 * - Currently a placeholder; actual attribute application can be implemented as needed.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_SANITIZE_THREAD_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_THREAD_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"


namespace LynxFunctionAttr {

    class SanitizeThreadHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;
                auto attr = llvm::Attribute::get(func->getContext(), "sanitize_thread");
                func->addFnAttr(attr);
            }
        };        
        
}

#endif

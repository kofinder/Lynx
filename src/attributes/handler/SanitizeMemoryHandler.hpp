/**
 * @file SanitizeMemoryHandler.hpp
 * @brief Handler that applies memory sanitizer attributes to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Intended to mark functions for memory sanitization checks.
 * - Currently a placeholder; actual attribute application can be implemented as needed.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_SANITIZE_MEMORY_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_MEMORY_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class SanitizeMemoryHandler : public FunctionAttributeHandler {

        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked SanitizeMemoryHandler");
            }
        };
             
        
}

#endif

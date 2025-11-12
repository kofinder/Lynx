/**
 * @file SanitizeCoverageHandler.hpp
 * @brief Handler that applies coverage sanitizer attributes to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Intended to mark functions for coverage instrumentation.
 * - Currently a placeholder; actual attribute application can be implemented as needed.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/


#ifndef LYNX_FUNC_SANITIZE_COVERAGE_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_COVERAGE_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"

#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class SanitizeCoverageHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked SanitizeCoverageHandler");
            }
        };
        
         
                
}

#endif

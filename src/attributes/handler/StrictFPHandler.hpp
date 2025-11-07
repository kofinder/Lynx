/**
 * @file StrictFPHandler.hpp
 * @brief Handler that applies the StrictFP attribute to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects if a function has the "strictfp" attribute.
 * - Applies the LLVM StrictFP attribute to enforce strict floating-point semantics.
 * - Ensures consistent floating-point behavior across targets.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_FUNC_STRICT_FP_HANDLER_HPP
#define LYNX_FUNC_STRICT_FP_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class StrictFPHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked StrictFPHandler");
                if (func->hasFnAttribute("strictfp")) {
                    builder.addAttribute(llvm::Attribute::StrictFP);
                    LOG_INFO("Applied strict-fp attributes");
                }
            }
        };     
                
}

#endif

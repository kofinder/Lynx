/**
 * @file NoInlineHintHandler.hpp
 * @brief Handler for functions marked with "no-inline-hint" attribute.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects LLVM functions annotated with the "no-inline-hint" attribute.
 * - Applies the NoInline LLVM attribute to prevent inlining based on hints.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_NO_INLINE_HIT_HANDLER_HPP
#define LYNX_FUNC_NO_INLINE_HIT_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"

#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class NoInlineHintHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoInlineHintHandler");
                if (func->hasFnAttribute("no-inline-hint")) {
                    builder.addAttribute(llvm::Attribute::NoInline);
                    LOG_WARN("Applied no-inline-hit attribute");
                }
            }
    };

}

#endif

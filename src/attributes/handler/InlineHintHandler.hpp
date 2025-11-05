/**
 * @file InlineHintHandler.hpp
 * @brief Handler for applying LLVM "InlineHint" attributes to functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects functions that should receive inline hints (e.g., based on name prefix).
 * - Improves optimization by suggesting LLVM to consider inlining specific functions.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_INLINE_HIT_HANDLER_HPP
#define LYNX_FUNC_INLINE_HIT_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class InlineHintHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked InlineHintHandler");
                if (func->getName().startswith("hint_inline")) {
                    builder.addAttribute(llvm::Attribute::InlineHint);
                    LOG_WARN("Applied hint_inline attributes");
                }
            }

    };
}        

#endif 

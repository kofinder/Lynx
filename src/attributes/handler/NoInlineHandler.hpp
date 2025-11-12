/**
 * @file NoInlineHandler.hpp
 * @brief Handler for functions that should not be inlined based on size or criteria.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects LLVM functions exceeding a size threshold (e.g., > 100 instructions).
 * - Applies the NoInline attribute to prevent inlining of large functions.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_NO_INLINE_HANDLER_HPP
#define LYNX_FUNC_NO_INLINE_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"

#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class NoInlineHandler : public FunctionAttributeHandler {

        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoInlineHandler");
                if (func->size() > 100) {
                    builder.addAttribute(llvm::Attribute::NoInline);
                    LOG_WARN("Applied no-inline attributes");
                }
            }
    };    

}

#endif

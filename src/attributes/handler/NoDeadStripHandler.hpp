/**
 * @file NoDeadStripHandler.hpp
 * @brief Handler for functions marked with the "no-dead-strip" attribute.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects LLVM functions with the "no-dead-strip" attribute.
 * - Optionally applies the corresponding LLVM function attribute to prevent dead code elimination.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_NO_DEAD_STRIP_HANDLER_HPP
#define LYNX_FUNC_NO_DEAD_STRIP_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;


    class NoDeadStripHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoke NoDeadStripHandler");
                if (func->hasFnAttribute("no-dead-strip")) {
                   // builder.addAttribute(llvm::Attribute::NoDeadStrip);
                }
            }
    };
    
}

#endif

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
 * @date: November 5, 2025
*/

#ifndef LYNX_FUNC_RETURN_TWICE_HANDLER_HPP
#define LYNX_FUNC_RETURN_TWICE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;


    class ReturnsTwiceHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked ReturnsTwiceHandler");
                if (func->getName().contains("setjmp")) {
                    LOG_WARN("Applied setjmp attributes");
                    builder.addAttribute(llvm::Attribute::ReturnsTwice);
                }
            }
        };
              
                
}

#endif

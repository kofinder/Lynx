/**
 * @file NoReturnHandler.hpp
 * @brief Handler that applies the `NoReturn` attribute to functions like abort or exit.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Detects functions that never return (e.g., abort, exit) and applies the LLVM `NoReturn` attribute.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/

#ifndef LYNX_FUNC_NO_RETURN_HANDLER_HPP
#define LYNX_FUNC_NO_RETURN_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;


    class NoReturnHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoReturnHandler");
                if (func->getName().startswith("abort") || func->getName().startswith("exit")) {
                    builder.addAttribute(llvm::Attribute::NoReturn);

                    LOG_WARN("Applied abort, exit attribute");
                }
            }
    };    

}

#endif

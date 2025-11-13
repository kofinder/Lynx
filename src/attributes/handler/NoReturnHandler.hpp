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

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class NoReturnHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                llvm::StringRef name = func->getName();
                if ((name.size() >= 5 && name.substr(0, 5) == "abort") ||
                    (name.size() >= 4 && name.substr(0, 4) == "exit")) {
                    builder.addAttribute(llvm::Attribute::get(func->getContext(), llvm::Attribute::NoReturn));
                    LOG_WARN("Applied 'noreturn' attribute to function {}", name.str());            
                }
            }
    };    

}

#endif
